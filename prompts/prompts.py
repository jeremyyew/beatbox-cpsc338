from . import sucker_prompts
from . import firefly_prompts
from . import shape_of_you_prompts
from . import nobody_prompts


def process_prompts(unprocessed_prompts):
    # {2: [[0, 1], [2, 3], [4, 5]]} ->
    # {2: [0, 1], 4: [2, 3], 6: [4, 5]}
    processed_prompts = {}
    for beat, sequences in unprocessed_prompts['prompts'].items():
        current = None
        for seq in sequences:
            if current is None:
                current = beat
            if seq is None:
                current += 1
                continue
            processed_prompts[current] = seq
            current += 1
    unprocessed_prompts['prompts'] = processed_prompts
    return unprocessed_prompts


def get_prompts():
    return {
        'firefly': process_prompts(firefly_prompts.get_unprocessed_prompts()),
        'sucker': sucker_prompts.get_song_prompts(),
        'shape_of_you': process_prompts(shape_of_you_prompts.get_unprocessed_prompts()),
        'nobody': nobody_prompts.get_song_prompts(),
    }
