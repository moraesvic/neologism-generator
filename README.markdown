This is a neologism generator. A [neologism](https://en.wikipedia.org/wiki/Neologism) is a freshly coined word. Although some creativity is involved in the creation of new words, as we properly credit the geniuses of Shakespeare, Joyce and Guimarães Rosa, neologisms follow phonological and morphological patterns which can be replicated by a mechanical process.

Starting from frequency words from natural languages, obtained from [this blog](https://invokeit.wordpress.com/frequency-word-lists/) referred by Wikipedia, we produce new words, that follow their patterns as a [Markov (anamnesic) process](https://en.wikipedia.org/wiki/Markov_chain). For example, the i-th letter in a word depends solely one the last N letters defined by the parameter "Trie-Depth". If you use a too low "Trie-Depth", you will end up with almost random words, as the next letter only depends on the last one or two. When the parameter goes up, we get words that look somewhat funny, but could eventually be found in the language, even if as a pun. With a high Trie-Depth, the words are probably proper words of the language, but were not in the original list because they were too specific (too many derivational affixes and so on).

(insert examples later)

The backend was implemented with C, with my own implementation of the Trie data structure. This makes it quite fast to process frequency lists. Most of the work is actually done trying to find a word which follows the given Markov process, but is not in the original list. Anyway, even in the worst reasonable scenarios, the server usually responds with only 100 MB RAM usage and 4 seconds delay.

The web-backend is implemented with NodeJS, with the library teen_process to call C code as an external process.

Give it a try on my homepage:
https://vulpi-ideas.com/neogen