#!/usr/bin/env python3

def main():
    num_players = 6
    num_unique_pieces = 9
    '''
    0: Pawn
    1: Bishop
    2: Knight
    3: Queen
    4: Rook
    5: Duchess
    6: Wizard
    7: Fortress
    8: (King)
    '''
    piece_weights = [
        0.06,
        0.07,
        0.05,
        0.13,
        0.09,
        0.11,
        0.06,
        0.12,
    ]

    partner_wizard_weights = [
        0.05,
        -0.05
    ]

    pawn_distance_weights = [0.001]

    bias_weight = 0.09

    # Note WCCI was for 4 players
    # We copy P3, P4 to P5, P6
    check_weights = [
        -0.05,
        0.08,
        -0.04,
        0.04,
        -0.04,
        0.04,
    ]
    checkmate_weights = [
        -0.33,
        0.26,
        -0.22,
        0.24,
        -0.22,
        0.24,
    ]

    weights = []

    # Materials
    for weight in piece_weights:
        weights.append(weight)

    # Partner wizards
    for weight in partner_wizard_weights:
        weights.append(weight)

    # Pawn distance weights
    for weight in pawn_distance_weights:
        weights.append(weight)

    # Bias weight
    weights.append(bias_weight)

    # Check weights
    for weight in check_weights:
        weights.append(weight)

    # Checkmate weights
    for weight in checkmate_weights:
        weights.append(weight)

    # print ("Total weights in array: " + str(len(weights)))
    print ' '.join(str(weight) for weight in weights)


if __name__ == "__main__":
    main()
