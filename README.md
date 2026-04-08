# Word Ladder CLI Game

This project is a command‑line C program that generates and solves word ladders—sequences of valid words where each step changes exactly one letter. The program can run interactively as a game or operate purely as a solver, depending on the arguments provided.

## Overview

The program loads a dictionary of words, selects or accepts start/end words, and either:

    Interactive mode: lets the user build a ladder step‑by‑step, validating each word. Computes the shortest possible ladder on exit

    Solver mode: automatically computes the shortest possible ladder between the two words.

It supports arbitrary word lengths (defaults to 4 letter words) as long as the dictionary contains words of that length.

## Features

- Build word ladders interactively

- Validate user‑entered words against the dictionary

- Automatically compute the shortest ladder using graph search

- Random or user‑specified start and end words

- Configurable dictionary file and word length

### Command‑Line Arguments

The program accepts several optional arguments:

    -d <filename> — specify the dictionary file

    -n <length> — specify the word length to use

    -s <word> — set the start word

    -f <word> — set the final word

    -p [ON|OFF] - sets interactive mode on or off

If start or end words are not provided, the program prompts the user to provide them or to allow random selection.
