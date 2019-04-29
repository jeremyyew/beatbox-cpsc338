import pygame
import time
import threading
import serial
from enum import Enum
from prompts import get_prompts
import sys

# TODO:
# Sat Night:
# -x circuit diagram
# - poster

# Sun:
# - transcribe a song
# - second song/game state?
# - enclosure
# - print poster
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

        while current_beat <= self.duration_beats:
            now_ms = (time.time() - self.start_time) * 1000
            current_beat = now_ms // self.BEAT_TIME_MS

            # Processing next beat
            if current_beat > self.last_updated:
                curr_prompts = self.prompts.get(current_beat)
                next_beat = current_beat + 1
                # print("BEAT", current_beat, curr_prompts)

                if current_beat in self.prompts:
                    # Update.
                    for button_num in self.prompts[current_beat]:
                        set_color(button_num, Color.ACTIVE)

                if next_beat in self.prompts:
                    for button_num in self.prompts[next_beat]:
                        set_color(button_num, Color.HINT)

                for button_num in buttons:
                    if current_beat not in self.prompts or button_num not in self.prompts[current_beat]:
                        if next_beat not in self.prompts or button_num not in self.prompts[next_beat]:
                            turn_off(button_num)

                self.last_updated = current_beat
                
            if ser.inWaiting() > 0:
                incoming = ser.read()
                action_num = ord(incoming) - 48
                if (action_num < 16):
                    self.button_pressed(action_num, current_beat)

        print("CORRECT:", self.correct)
        print("WRONG:", self.wrong)
        print("FINAL SCORE:",self.correct - self.wrong)


    def button_pressed(self, button_num, current_beat):
        print(f'BEAT {current_beat} pressed {button_num} prompts {self.prompts.get(current_beat)}')
        if current_beat in self.prompts and button_num in self.prompts[current_beat]:
            # Record score.
            self.correct += 1
            new_color = Color.CORRECT          # Correct when pressed after prompt
        else:
            self.wrong += 1
            new_color = Color.WRONG             # Wrong if you press and you weren't supposed to
            
        set_color(button_num, new_color)


ser = serial.Serial(PORT, 115200, timeout=0)
all_songs = get_prompts()
info = all_songs['firefly']

if len(sys.argv) > 1:
    if sys.argv[1] not in all_songs:
        print('Unknown song')
        exit()
    else:
        info = all_songs[sys.argv[1]]

print('Initializing...')
buttons = range(0, 16)

time.sleep(3)  # allow time to initalize serial (restarts sketch)

game = Game(title=info['song_file'], tempo=info['tempo'],
            prompts=info['prompts'], duration_beats=info['duration'])
game.start()
