IMPORTANT:
In order to use the first function, you need to download "Default Cards" from https://scryfall.com/docs/api/bulk-data. Add it to the same folder this program is running in, and make sure it's named "default-cards.json". 

This program has 2 functions:
1. Processes all magic cards from scryfall's bulk data file, filters them based on programmed conditions (currently, only filters commander-legal non-land cards), then outputs the names to a text file.
2. Using the list of filtered cards, generates a new file with a number of random cards from the previous file, based on user input.
