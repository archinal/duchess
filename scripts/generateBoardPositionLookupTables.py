#!/usr/bin/env python3

# Generates lookup tables to translate human readable positions for Duchess (e.g. 3c4) into an array index or vice versa
# These lookup tables are intended to be copied into C++ source files so they are loaded at compile time

import argparse


def main():
    args = parse_args()
    if args.num_players == "6":
        if args.output_type == 'index':
            print(six_player_positions_to_index())
        elif args.output_type == 'positions':
            print(six_player_index_to_position())
        else:
            print("Error: --output-type must be [index|positions]")
    else:
        print("Error: Only 6 player conversions possible")


def parse_args():
    parser = argparse.ArgumentParser(description="Generate Duchess position <--> array index lookup tables")

    parser.add_argument("--num-players", required=True)
    parser.add_argument("--output-type", required=True)
    args = parser.parse_args()

    return args


def get_num_valid_positions_six_player():
    flaps = 6
    rows = 5
    columns = 7

    return flaps * rows * columns


def get_num_board_indices_six_player():
    squares_in_flaps = 6 * 5 * 4
    squares_in_hex_owned_by_flap = 6 * 6
    vortex = 1
    return squares_in_flaps + squares_in_hex_owned_by_flap + vortex


# Given any position, array converts to a board index
def six_player_positions_to_index():
    # Indexed as follows:
    # First 6 * 5 * 4 are for squares in flaps, indexed as flap/column/row
    # So 2c4 is at (2 * 5 * 4)
    # If position is OB, look up at 0
    # So for column <= 4, look up at ((flap - 1) * 5 * 4) + ((col - 1) * 5) + row
    # For 4 < column <= 6, look up at 120 + ((flap - 1) * 3 * 2) + ((col - 1) * 3) + row
    # This array has get_num_valid_positions_six_player elements
    board_indices = [0]
    num_flaps = 6
    num_rows = 5
    num_columns_in_flap = 4
    num_columns_out_flap = 2
    squares_per_flap = 5 * 4

    # Discrete flap entries
    for flap in range(1, num_flaps + 1):
        for col in range(1, num_columns_in_flap + 1):
            for row in range(1, num_rows + 1):
                board_indices.append(((flap - 1) * num_rows * num_columns_in_flap) + ((col - 1) * num_rows) + row)

    hexagon_offset = num_flaps * num_rows * num_columns_in_flap

    # Hexagon entries (with overlap)
    for flap in range(1, num_flaps + 1):
        for col in range(1, num_columns_out_flap + 1):
            for row in range(1, num_rows + 1):
                modified_flap = flap
                modified_row = row
                modified_col = col
                if row < 3:
                    # Not "owned" by this flap
                    # Modify flap
                    modified_flap += 1
                    if modified_flap > num_flaps:
                        modified_flap -= num_flaps
                    # Swap row, col
                    modified_row = 6 - col
                    modified_col = row

                board_indices.append(hexagon_offset + ((modified_flap - 1) * 3 * num_columns_out_flap) + ((modified_col - 1) * 3) + modified_row - 2)

    # Now the vortex, the final index
    board_indices.append(get_num_board_indices_six_player())
    return board_indices


# Given a board index, converts to a position
def six_player_index_to_position():
    # Off Board
    positions = ["OB", "", ""]

    for i in range(1, get_num_board_indices_six_player()):
        if i <= 120:
            # Index refers to a discrete flap
            flap = int((i - 1) / 20) + 1
            if i % 20 == 0:
                col = 4
            else:
                col = int(((i % 20) - 1) / 5) + 1
            row = (i % 20) % 5
            if row == 0:
                row += 5
        else:
            # Index refers to the central hexagon
            j = i - 120
            flap = int((j - 1) / 6) + 1
            if j % 6 == 0:
                col = 6
            else:
                col = int(((j % 6) - 1) / 3) + 5
            row = (j % 6) % 3
            if row == 0:
                row += 3
            row += 2

        positions.append(flap)
        positions.append(row_to_char(row))
        positions.append(col)

    # Vortex
    positions.append("V")
    positions.append("")
    positions.append("")

    return positions

def row_to_char(row):
    chars = ["a", "b", "c", "d", "e"]
    return chars[row - 1]


def flap_row_col_to_position(flap, row, col):
    return str(flap) + row_to_char(row) + str(col)

if __name__ == "__main__":
    main()
