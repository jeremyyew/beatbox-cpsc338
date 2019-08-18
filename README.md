# beatbox-cpsc338
![Content warning: rapidly flashing lights.](docs/demo.gif)

A highly addictive rhythm game with very aesthetic and squishable buttons. Hit the buttons as they light up in time to the music! 

## Demo video (turn sound on)
*Content warning: rapidly flashing lights.*

Song: Nobody by James Arthur, Martin Jensen. 

[![Beatbox demo](http://img.youtube.com/vi/BtYXU2IIU3U/0.jpg)](https://www.youtube.com/watch?v=BtYXU2IIU3U "Beatbox demo")


## Game instructions
Press the buttons in beat to the music. Some patterns will repeat but get more complicated as the song progresses. 

- When a button turns **dark blue**, it's hinting that it's about to turn active in the next beat. 
- When a button turns **light blue**, it's active, so hit that button within that beat to score a point. 
  - If you pressed it in time, it will turn **purple**.
  - Too early or too late and it will turn **red**, and you lose a point. 

Your final score will be printed on the screen. Have fun!

## Setup and play 
Connect Beatbox and laptop via USB. 
Run `python game.py <song_name>`, e.g. `python game.py firefly`. 

Songs included: (found in `/prompts`)
- `firefly`: Fireflies by Owl City (Difficulty: Easy)
- `sucker`: Sucker by Jonas Brothers (Difficulty: Medium)
- `nobody`: Nobody by James Arthur, Martin Jensen (Difficulty: Hard)
- `shape_of_you`: Shape of You by Ed Sheeran, Galantis Remix (Difficulty: Hard)

# Highlights 
## Message system between CPU and Arduino 
We send single-byte messages over serial communication, so that we don't have to define a custom multiple-bytes protocol. That means we can encode the numbers `0-256`.

### Arduino to CPU
We have 16 buttons, and 2 possible message (`PRESSED`, `RELEASED`) for the Arduino to send to the PC that correspond to 2 different button states. So we only use `0-31`. 

For `CORRECT` or `WRONG` feedback, we only light up while the button is pressed, instead of for a duration, that's why we register button releases as well.  

- `0-15`: button `N` has been `PRESSED`.
- `16-31`: button `N` has been `RELEASED`.


### CPU to Arduino 
The CPU either:
-  tells the Arduino to light up (`HINT`, `ACTIVE`) and turn off (`OFF`) buttons on the keypad at specific beats. 
-  receives messages from the Arduino that a button has been pressed, checks whether it was pressed in the correct time window, and then sends the feedback `CORRECT` or `WRONG` for the button to light up.

We have 16 buttons, and 5 possible actions (`OFF`, `WRONG`, `ACTIVE`,`HINT`, and `CORRECT`) for the PC to send to the Arduino that correspond to 5 different button LED states, so we'll have 5 ranges of size 16, and `5 x 16 = 80`. So we only use `0-79`. 

- `0-15`: set button `N` to `OFF` (no light).
- `16-31`: set button `N` to `WRONG` (red). 
- `32-47`: set button `N` to `ACTIVE` (bright blue).
- `48-63`: set button `N` to `HINT` (dark blue).
- `64-79`: set button `N` to `CORRECT` (purple).
## Composing new song sequences 
### Sequence representation
We want a way to represent an exactly timed sequence of prompts, i.e. the buttons lighting up. We couldn't find relevant examples online so we had to design it ourselves. 

Our requirements: 
- It should be **readable, writable, and modular**. This will make it easier to write new songs sequences, both for us and for any user. 
- For example, we don't want a sparse array, which would be hard to read and modify. 
- Neither do we want to label every single prompt with a timestamp, or copy-paste repeated segments, which would be hard to write and modify as well. Inserting one beat would mean re-labelling all following beats. 
- Even if the final data read by the program is 'ugly', we want a preprocessed version that is 'pretty'.

Thus, we implement a simple **beat-timed, interval-based representation**. 

First, since every prompt will naturally be in sync to the beat of the song, we abstract over different song speeds by obtaining the particular beat-time of the song. We use the **tempo** of the song, usually recorded as beats-per-minute (sometimes in the song file, sometimes found online) to calculate the **duration of each beat in milliseconds**. This allows us to keep track of which beat we are currently on at runtime, and light up the corresponding buttons for that beat. 

```python
self.BEAT_TIME_MS = 1000 * 60/tempo
current_beat = now_ms // self.BEAT_TIME_MS
```
For example, a 3-minute song with 120BPM will have beats of 500ms each, and a total of 360 beats.  

Still, this requires every beat to be labeled `0` to `359`, which is annoying. Thus, we let preprocessed subsequences (or, "intervals") of beats be implicitly consecutive, as long as the interval itself is labeled by its starting beat; then we simply define a processor that labels every beat in the interval consecutively. Beats with no prompts, or 'empty' beats, may then be either represented by a `None`, or implicitly by the gaps in between the intervals. 

Here is an example song prompt file, from [prompts/firefly_prompts.py](prompts/firefly_prompts.py), which completely describes the song sequence and required metadata. 
The first interval is the `intro`, to be inserted at beat `0`; the first beat `None` means an empty beat; the second beat `[13,14]` means buttons 13 and 14 should prompt, and so on. 
```python
def get_preprocessed_prompts():
    intro = [
        None, [13, 14], None, [12, 15],
        None, [9, 10], None, [8, 11],
        None, [13, 14], None, [12, 15],
        None, [9, 10], None, [8, 11],
        # ...
    ]
    verse = [
        [5, 6], None, [4, 7], [8, 11],
        [4, 7], None, [8, 11], [12, 15],
        [8, 11], None, [4, 7], [8, 11],
        [4, 7], None, [8, 11], [12, 15],
        # ...
    ]
    chorus = [
        [5, 10], [6, 9], [4, 11], [8, 7],
        [5, 10], [6, 9], [4, 11], [8, 7],
        [0, 15], None, [3, 12], None,
        [1, 14], None, [13, 2], None,
        # ...
    ]
    firefly_prompts = {
        0: intro, 32: verse, 64: chorus,
        96: verse, 128: chorus,
    }
    return {
        'prompts': firefly_prompts,
        'duration': 160, 'tempo': 90,
        'song_file': 'firefly.mp3'
    }
```
Notice that the verse and chorus are easily repeated, and all intervals may be easily rearranged. The intervals here are injected at 32-beat intervals, which happens to be their exact length, but there might be other cases which require irregular or non-consecutive intervals. 

Also, in the song sequences we wrote, every prompt lasts exactly one beat, but it would be trivial to implement longer or shorter prompt durations. 

### Auto-compose: write new sequences by playing them out 
We also implemented an "auto-compose" feature which lets the user write prompts for new songs by playing them out physically on the device (see [recorder.py](recorder.py)), thus avoiding the manual work of mentally translating button numbers, or counting beats. 

The device records a sequence of button presses, which may be out of time, and 'syncs' them by labelling each one as occuring one beat after another. It then prints them out. These can be easily copy-pasted to form an entire new song.

Both the songs 'Nobody' and 'Sucker' were recorded this way. 

## Implementation details
1. Set up I2C communication between Arduino and NeoTrellis keypad. 
2. Arduino polls button state and lights up buttons on keypad with Trellis library.
3. Set up serial communication between Arduino and PC, using `Serial.write()` on Arduino and `pyserial` for PC.
4. Define single-byte serial messages (light up button, button pressed). 
5. PC runs game driver code, telling the Arduino which buttons to light up, while Arduino tells PC when buttons are pressed. 
6. Driver code processes data object to play notes in time to music.

### Game driver pseudocode
```
setup serial communication
load song file and button prompts
start music
for duration of song:
   calculate current beat based on time elapsed and beats/second
   if moving onto new beat:
       light up buttons that should be pressed for current beat
       light up buttons to hint for next beat
       turn off all other button lights
       read serial input to process incoming button presses
       change button color based on correct/incorrect button press, and record score
calculate and display final score
```

## Project poster 
![Poster](docs/button_hero_poster.png)

## Circuit schematic
<img src="docs/circuit_diagram.png" width="200">

## Bill of materials
- Adafruit NeoTrellis RGB Driver PCB for 4x4 Keypad (RGB LEDs included) https://www.adafruit.com/product/3954 
- Silicone Elastomer 4x4 Button Keypad for 3mm LEDs https://www.adafruit.com/product/1611
- USB 2.0 CABLE TYPE A/B https://store.arduino.cc/usa/usb-2-0-cable-type-a-b

## References 
- Connect NeoTrellis to arduino: https://learn.adafruit.com/adafruit-neotrellis 