## What is this?

This is a neologism generator. A [neologism](https://en.wikipedia.org/wiki/Neologism) is a freshly coined word. Although some creativity is involved in the creation of new words, as we properly credit the geniuses of Shakespeare, Joyce and Guimarães Rosa, neologisms follow phonological and morphological patterns which can be replicated by a mechanical process.

Starting from frequency words from natural languages, obtained from [this blog](https://invokeit.wordpress.com/frequency-word-lists/) referred by Wikipedia, we produce new words, that follow their patterns as a [Markov (anamnesic) process](https://en.wikipedia.org/wiki/Markov_chain). For example, the i-th letter in a word depends solely one the last N letters defined by the parameter "Trie-Depth". If you use a too low "Trie-Depth", you will end up with almost random words, as the next letter only depends on the last one or two. When the parameter goes up, we get words that look somewhat funny, but could eventually be found in the language, even if as a pun. With a high Trie-Depth, the words are probably proper words of the language, but were not in the original list because they were too specific (too many derivational affixes and so on).

## Examples

**English: generate words with a trie-depth of 3**: joken, mattoo, worrow
**English: generate words with a trie-depth of 5**: caughter, knowledged, bundless
**English: generate words with a trie-depth of 10**: air-conditioner, understandings, unconvention, indistinguish, misinterpreter
**Portuguese: generate words with a trie-depth of 5**: profissionado, massassinando, encontratou

## How was it implemented?

The backend was implemented with C, with my own implementation of the Trie data structure. This makes it quite fast to process frequency lists. Most of the work is actually done trying to find a word which follows the given Markov process, but is not in the original list. Anyway, even in the worst reasonable scenarios, the server usually responds with only 100 MB RAM usage and 4 seconds delay.

The web-backend is implemented with NodeJS.

## Live demonstration

Give it a try on my homepage (still not fully integrated with the rest of the website):
https://vulpi-ideas.com/neogen