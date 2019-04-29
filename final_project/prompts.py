<<<<<<< HEAD
def get_prompts():
    # Layout:
    # 15,14,13,12
    # 11,10, 9, 8
    #  7, 6, 5, 4
    #  3, 2, 1, 0
    intro = [
        None, [13, 14], None, [12, 15],
        None, [13, 14], [9, 10], [8, 11],
        None, [13, 14], None, [12, 15],
        None, [13, 14], [9, 10], [8, 11],

        None, [13, 14], None, [12, 15],
        None, [1], [5], [9],
        None, [2], [6], [10],
        None, [13, 14], [9, 10], [8, 11],
    ]
    verse = [
        [5], [6], [9, 10], [12, 15],
        [9], [10], [5, 6], [0, 3],
        [5], [6], [9, 10], [12, 15],
        [9], [10], [5, 6], [0, 3],

        [9, 10], None, [8, 11], [4, 7]
        [8, 11], None, [4, 7], [0, 3],
        [4, 7], None, [8, 11], [4, 7]
        [8, 11], None, [4, 7], [0, 3],
    ]
    chorus = [
        [5, 10], [6, 9], [4, 11], [8, 7],
        [5, 10], [6, 9], [4, 11], [8, 7],
        [0, 15], None, [3, 12], None,
        [1, 14], None, [13, 2], None,

        [5, 10], [6, 9], [4, 11], [8, 7],
        [5, 10], [6, 9], [4, 11], [8, 7],
        [0, 15], None, [3, 12], None,
        [1, 14], None, [13, 2], None,
    ]
    firefly_prompts = {
        # intro
        0: intro,
        # verse
        32: verse,
        # chorus
        64: chorus
    }
    return {
        'prompts': process_prompts(firefly_prompts),
        'duration': 194}

=======
import sucker_prompts
import firefly_prompts
>>>>>>> 389f38d707386dbb599752f1a99c5cf7d62427a9

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
    }



