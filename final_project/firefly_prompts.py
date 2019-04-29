def get_unprocessed_prompts():
    # Layout:
    # 15,14,13,12
    # 11,10, 9, 8
    #  7, 6, 5, 4
    #  3, 2, 1, 0
    firefly_prompts = {
        # intro
        0: [
            None, [1, 2], None, [0, 3],
            None, [5, 6], None, [4, 7],
            None, [1, 2], None, [0, 3],
            None, [5, 6], None, [4, 7],

            None, [1, 2], None, [0, 3],
            None, [5, 6], None, [4, 7],
            None, [1, 2], None, [0, 3],
            None, [5, 6], None, [4, 7],
        ],
        # verse
        32: [
            [5, 6], None, [4, 7], [8, 11],
            [4, 7], None, [8, 11], [12, 15],
            [8, 11], None, [4, 7], [8, 11],
            [4, 7], None, [8, 11], [12, 15],

            [8, 11], None, [4, 7], [8, 11],
            [4, 7], None, [8, 11], [12, 15],
            [8, 11], None, [4, 7], [8, 11],
            [4, 7], None, [8, 11], [12, 15],
        ],
        # chorus
        64: [
            [0, 3], None, [4, 7], None,
            [8, 11], None, [12, 15], None,
            [5, 6], [1, 2], [0, 3], None,
            [9, 10], [13, 14], [12, 15], None,

            [4, 7], None, [6], [5],
            [8, 11], None, [9], [10],
            [6, 9], [5, 10], [7, 8], [4, 11],
            [12, 15], None, None, None,
        ]
    }
    return {
        'prompts': firefly_prompts,
        'duration': 180,
        'tempo': 90,
        'song_file': 'sounds/firefly.mp3'
    }
