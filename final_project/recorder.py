import pygame
import time
import threading
import serial
from enum import Enum

PORT = '/dev/cu.usbmodem1421'
music = pygame.mixer.music


class Color(Enum):
    OFF = 0
    WRONG = 1
    ACTIVE = 2
    HINT = 3
    CORRECT = 4


def set_color(button_num, color):
    ser.write(bytes(chr(button_num + 48 + 16 * color.value), 'utf-8'))


def turn_off(button_num):
    ser.write(bytes(chr(button_num + 48 + 16 * Color.OFF.value), 'utf-8'))


class Game:
    def __init__(self, title, tempo, prompts, duration_beats):
        pygame.init()
        music.load(title)
        self.prompts = prompts
        self.BEAT_TIME_MS = 1000 * 60/tempo
        self.last_updated = -1
        self.correct = 0
        self.wrong = 0
        self.duration_beats = duration_beats

    def start(self):
        music.play()
        self.start_time = time.time()
        current_beat = 0

        toggle = 0

        while current_beat <= self.duration_beats:
            now_ms = (time.time() - self.start_time) * 1000
            current_beat = now_ms // self.BEAT_TIME_MS

            # Processing next beat
            if current_beat > self.last_updated:
                print("BEAT", current_beat)
                self.last_updated = current_beat

                for button_num in buttons:
                    turn_off(button_num)

            if ser.inWaiting() > 0:
                incoming = ser.read()
                action_num = ord(incoming) - 48
                if (action_num < 16):
                    self.button_pressed(action_num, current_beat)

        print(recording)

    def button_pressed(self, button_num, current_beat):
        print(f'BEAT {current_beat} pressed {button_num}')
        set_color(button_num, Color.ACTIVE)
        beat_buttons = recording.get(current_beat)
        if beat_buttons is not None:
            beat_buttons.append(button_num)
        else:
            beat_buttons = [button_num]

        recording[current_beat] = beat_buttons


ser = serial.Serial(PORT, 115200, timeout=0)
print('Initializing...')
buttons = range(0, 16)

time.sleep(3)  # allow time to initalize serial (restarts sketch)

recording = {}

game = Game(title='sounds/nobody.mp3', tempo=98,
            prompts=None, duration_beats=83)
game.start()
