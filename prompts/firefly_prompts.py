def get_unprocessed_prompts():
    # Layout:
    intro = [
        None, [13, 14], None, [12, 15],
        None, [9, 10], None, [8, 11],
        None, [13, 14], None, [12, 15],
        None, [9, 10], None, [8, 11],

        None, [13, 14], None, [12, 15],
        None, [13, 14], [9, 10], [8, 11],
        None, [13, 14], None, [12, 15],
        None, [13, 14], [9, 10], [8, 11]
    ]
    verse = [
        [5, 6], None, [4, 7], [8, 11],
        [4, 7], None, [8, 11], [12, 15],
        [8, 11], None, [4, 7], [8, 11],
        [4, 7], None, [8, 11], [12, 15],

        [5], [6], [9, 10], [12, 15],
        [9], [10], [5, 6], [0, 3],
        [5], [6], [9, 10], [12, 15],
        [9], [10], [5, 6], [0, 3]
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
        0: intro,
        32: verse,
        64: chorus,
        96: verse,
        128: chorus,
    }
    return {
        'prompts': firefly_prompts,
        'duration': 160,
        'tempo': 90,
        'song_file': 'firefly.mp3'
    }
