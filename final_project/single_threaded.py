import pygame, time, threading
import serial
from enum import Enum
from collections import OrderedDict

music = pygame.mixer.music

firefly_prompts = {
    # beat number : key buttons that should be pressed within beat
    # 1: [1],
    # 2: [2, 3],            # can't have holds since the user wouldn't know to press twice?
    # 3: [3, 4, 8, 9],
    # 4: [4],
    # 8: [1]
    1: [1],
    2: [2],
    3: [1],
    4: [2],
    5: [1],
    6: [2]

}

class Color(Enum):
   OFF = 0
   A = 1
   B = 2
   C = 3


def set_color(button_num, color):
    button_states[button_num] = color
    print("lighting up:", button_num, color, button_num + 48 + 16 * color.value)
    ser.write(bytes(chr(button_num + 48 + 16 * color.value), 'utf-8'))

def turn_off(button_num):
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
        # music.play()
        self.start_time = time.time()
        current_beat = 0
        print("Song start time:", self.start_time)
        while current_beat <= self.duration_beats:
            now_ms = (time.time() - self.start_time) * 1000
            current_beat = now_ms // self.BEAT_TIME_MS

            # Processing next beat
            if current_beat > self.last_updated:
                print("BEAT", current_beat)
                # if self.last_updated in self.prompts:
                for button_num in buttons:
                    if current_beat not in self.prompts or button_states[button_num] not in self.prompts[current_beat]:
                        turn_off(button_num)

                self.last_updated = current_beat                
                # Check if there were any prompts that were not triggered. 
                # old_prompt = self.active_prompts
                # self.missed += len(old_prompt)
                # self.active_prompts = set()

                if current_beat in self.prompts:
                    # Update. 
                    self.active_prompts = set(self.prompts[current_beat])
                    # print("updating active prompts:", self.active_prompts)
                    for button_num in self.prompts[current_beat]:
                        set_color(button_num, Color.C)

            if ser.inWaiting() > 0: 
                incoming = ser.read()
                action_num = ord(incoming) - 48
                if (action_num < 16):
                    self.button_pressed(action_num)
                else:
                    self.button_released(action_num - 16)

        print(performance)
        print("CORRECT:", self.correct)
        print("MISSED:", self.missed)
        print("FINAL SCORE:",self.correct - self.missed)

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


ser = serial.Serial('/dev/cu.usbmodem1411', 115200, timeout=0)
print('Initializing...')
buttons = range(0, 16)
button_states = {key: Color.OFF for key in buttons}
time.sleep(3) # allow time to initalize serial (restarts sketch)

performance = {beat: [] for beat in firefly_prompts.keys()}

game = Game(title='sounds/fireflies.mp3',tempo=90,prompts={beat: set(firefly_prompts[beat]) for beat in firefly_prompts},duration_beats = 15)
print(firefly_prompts)
game.start()

# pygame.mixer.init(frequency=22050,size=-16,channels=4)
# sound1 = pygame.mixer.Sound('sounds/firefly-midi-2.ogg')
# chan1 = pygame.mixer.Channel(0)
# chan2 = pygame.mixer.Channel(1)
# pygame.init()
#chan1.queue(sound1)
#chan2.queue(sound2)
