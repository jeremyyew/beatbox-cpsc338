def get_prompts():
    # Layout:
    # 15,14,13,12
    # 11,10, 9, 8
    #  7, 6, 5, 4
    #  3, 2, 1, 0
    intro = [
        None, [1, 2], None, [0, 3],
        None, [5, 6], None, [4, 7],
        None, [1, 2], None, [0, 3],
        None, [5, 6], None, [4, 7],

        None, [1, 2], None, [0, 3],
        None, [5, 6], None, [4, 7],
        None, [1, 2], None, [0, 3],
        None, [5, 6], None, [4, 7],
    ]
    verse = [
        [5, 6], None, [4, 7], [8, 11],
        [4, 7], None, [8, 11], [12, 15],
        [8, 11], None, [4, 7], [8, 11],
        [4, 7], None, [8, 11], [12, 15],

        [8, 11], None, [4, 7], [8, 11],
        [4, 7], None, [8, 11], [12, 15],
        [8, 11], None, [4, 7], [8, 11],
        [4, 7], None, [8, 11], [12, 15],
    ]
    chorus = [
        [0, 3], None, [4, 7], None,
        [8, 11], None, [12, 15], None,
        [5, 6], [1, 2], [0, 3], None,
        [9, 10], [13, 14], [12, 15], None,

        [4, 7], None, [6], [5],
        [8, 11], None, [9], [10],
        [6, 9], [5, 10], [7, 8], [4, 11],
        [12, 15], None, None, None,
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
        'duration': 190}


def process_prompts(prompts):
    # {2: [[0, 1], [2, 3], [4, 5]]} ->
    # {2: [0, 1], 4: [2, 3], 6: [4, 5]}
    processed_prompts = {}
    for beat, sequences in prompts.items():
        current = None
        for seq in sequences:
            if current is None:
                current = beat
            if seq is None:
                current += 1
                continue
            processed_prompts[current] = seq
            current += 1
    print(processed_prompts)
    return processed_prompts
