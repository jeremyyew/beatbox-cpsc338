import sucker_prompts
import firefly_prompts

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



