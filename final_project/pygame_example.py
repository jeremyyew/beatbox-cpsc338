import pygame, time, threading
music = pygame.mixer.music

firefly_prompts = {
    # beat number : key buttons that should be pressed within beat
    1: [1],
    2: [2, 3],
    3: [3, 4],
    4: [4],
    8: [1]
    }


class Song:
    def __init__(self, title, tempo, prompts, duration_beats):
        pygame.init()
        music.load(title)
        self.prompts = prompts
        self.BEAT_TIME_MS = 1000 * 60/tempo
        self.active_prompts = []
        self.score = 0
        self.last_updated = -1
        self.duration_beats = duration_beats

    def start(self):
        music.play()
        self.start_time = time.time()
        while True:
            self.update_active_prompts()
        
    def update_active_prompts(self):
        # Updates once when we are in a new beat.
        now_ms = (time.time() - self.start_time) * 1000
        current_beat = now_ms // self.BEAT_TIME_MS
        if current_beat > self.duration_beats: 
            print("FINAL SCORE:",self.score)
            return
        # If we haven't updated for this beat, and there are prompts for the current beat. 
        if current_beat > self.last_updated and current_beat in self.prompts:
            # Check if there were any prompts that were not triggered. 
            # self.score -= 10
            # Update. 
            self.active_prompts = self.prompts[current_beat]
            self.last_updated = current_beat
            print("updating active prompts:", self.active_prompts)

    def get_duration_beats(self):
        return self.duration_beats
    def get_active_prompts():
        return self.active_prompts

class Game:
    def __init__(self, song):
        self.song = song
        self.duration_beats = song.get_duration_beats()


    def start(self):
        t1 = threading.Thread(target=self.song.start)
        t2 = threading.Thread(target=self.input_listen)
        t1.start()
        t2.start()

    def input_listen(self):
        pass
        # We want to read inputs here, and check if they are in current active prompts

        # start_time = time.time()
        # last_pressed = -1
        # time.sleep(0.1)
        # while True:
        #     now_ms = (time.time() - start_time) * 1000
        #     current_beat = now_ms / BEAT_TIME_MS
        #     if current_beat > duration: 
        #         return
        #     if current_beat > last_pressed and current_beat in prompts:
        #         for i in prompts[current_beat]:
        #             button_pressed(i)
        #         last_pressed = current_beat

    def button_pressed(self,button_num):
        self.active_prompts = self.song.get_active_prompts()
        score
        print(f'pressed {button_num}')
        if button_num in active_prompts:
            print(f'prompt {button_num} triggered')
            # Remove prompt so we can't trigger it again. 
            active_prompts.remove(button_num)
            # Record score.
            score += 100

    def button_released(self,button_num):
        print("released:", button_num)

fireflies = Song(title='sounds/fireflies.mp3',tempo=90,prompts=firefly_prompts,duration_beats=60)
game = Game(fireflies)
game.start()

# pygame.mixer.init(frequency=22050,size=-16,channels=4)
# sound1 = pygame.mixer.Sound('sounds/firefly-midi-2.ogg')
# chan1 = pygame.mixer.Channel(0)
# chan2 = pygame.mixer.Channel(1)
# pygame.init()
#chan1.queue(sound1)
#chan2.queue(sound2)
