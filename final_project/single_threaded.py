import pygame
import time
import threading
import serial
from enum import Enum
from collections import OrderedDict
from prompts import get_prompts

# TODO:
# Sat Night:
# -x circuit diagram
# - poster

# Sun:
# - transcribe a song
# - second song/game state?
# - enclosure
# - print poster
PORT = '/dev/cu.usbmodem1411'
music = pygame.mixer.music

firefly_prompts = get_prompts()


class Color(Enum):
    OFF = 0
    A = 1
    B = 2
    C = 3


def set_color(button_num, color):
    button_states[button_num] = color
    print("lighting up:", button_num, color,
          button_num + 48 + 16 * color.value)
    ser.write(bytes(chr(button_num + 48 + 16 * color.value), 'utf-8'))


def turn_off(button_num):
    print("turning off:", button_num)
    ser.write(bytes(chr(button_num + 48 + 16 * Color.OFF.value), 'utf-8'))


class Game:
    def __init__(self, title, tempo, prompts, duration_beats):
        pygame.init()
        music.load(title)
        self.prompts = prompts
        self.BEAT_TIME_MS = 1000 * 60/tempo
        self.active_prompts = set()
        self.last_updated = -1
        self.correct = 0
        self.missed = 0
        self.duration_beats = duration_beats

    def start(self):
        music.play()
        self.start_time = time.time()
        current_beat = 0
        print("Song start time:", self.start_time)
        while current_beat <= self.duration_beats:
            now_ms = (time.time() - self.start_time) * 1000
            current_beat = now_ms // self.BEAT_TIME_MS

            # Processing next beat
            if current_beat > self.last_updated:
                print("BEAT", current_beat)
                for button_num in buttons:
                    if current_beat not in self.prompts or button_num not in self.prompts[current_beat]:
                        turn_off(button_num)

                self.last_updated = current_beat

                # TODO: Check if there were any prompts that were not triggered.

                if current_beat in self.prompts:
                    # Update.
                    self.active_prompts = set(self.prompts[current_beat])
                    for button_num in self.prompts[current_beat]:
                        set_color(button_num, Color.C)

                next_beat = current_beat + 1
                if next_beat in self.prompts:
                    for button_num in self.prompts[next_beat]:
                        set_color(button_num, Color.A)

            if ser.inWaiting() > 0:
                incoming = ser.read()
                action_num = ord(incoming) - 48
                if (action_num < 16):
                    self.button_pressed(action_num)
                else:
                    self.button_released(action_num - 16)

        print("CORRECT:", self.correct)
        print("MISSED:", self.missed)
        print("FINAL SCORE:", self.correct - self.missed)

    def button_pressed(self, button_num):
        print(f'pressed {button_num}')
        new_color = Color.A             # Red if you press and you weren't supposed to
        if button_num in self.active_prompts:
            print(f'prompt {button_num} triggered')
            # Remove prompt so we can't trigger it again.
            self.active_prompts.remove(button_num)
            # Record score.
            self.correct += 1

            new_color = Color.B          # Green when pressed after prompt

        set_color(button_num, new_color)
        print(f'handled {button_num} pressed')

    def button_released(self, button_num):
        print("released:", button_num)


ser = serial.Serial(PORT, 115200, timeout=0)
print('Initializing...')
buttons = range(0, 16)
button_states = {key: Color.OFF for key in buttons}
time.sleep(3)  # allow time to initalize serial (restarts sketch)

# performance = {beat: [] for beat in firefly_prompts.keys()}

game = Game(title='sounds/fireflies.mp3', tempo=90,
            prompts=prompts.firefly_prompts, duration_beats=45)
print(prompts.firefly_prompts)
game.start()
