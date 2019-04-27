import pygame, os, time

pygame.init()
pygame.mixer.init(frequency=22050,size=-16,channels=4)

sound1 = pygame.mixer.Sound('sounds/firefly-midi-2.ogg')
# sound2 = pygame.mixer.Sound('sounds/punch.wav')
chan1 = pygame.mixer.Channel(0)
# chan2 = pygame.mixer.Channel(1)
chan1.queue(sound1)
# chan2.queue(sound2)
time.sleep(60)
while True:
    pass
