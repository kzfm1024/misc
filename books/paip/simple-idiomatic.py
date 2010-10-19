#
# http://norvig.com/python-lisp.html
#

"""Module to generate random sentences from a grammar.  The grammar
consists of entries that can be written as S = 'NP VP | S and S',
which gets translated to {'S': [['NP', 'VP'], ['S', 'and', 'S']]}, and
means that one of the top-level lists will be chosen at random, and
then each element of the second-level list will be rewritten; if it is
not in the grammar it rewrites as itself.  The functions rewrite and
rewrite_tree take as input a list of words and an accumulator (empty
list) to which the results are appended.  The function generate and
generate_tree are convenient interfaces to rewrite and rewrite_tree
that accept a string (which defaults to 'S') as input."""

import random

def make_grammar(**grammar):
  "Create a dictionary mapping symbols to alternatives."
  for k in grammar.keys():
    grammar[k] = [alt.strip().split() for alt in grammar[k].split('|')]
  return grammar
  
grammar = make_grammar(
  S = 'NP VP',
  NP = 'Art N',
  VP = 'V NP',
  Art = 'the | a',
  N = 'man | ball | woman | table',
  V = 'hit | took | saw | liked'
  )

def rewrite(words, into):
  "Replace each word in the list with a random entry in grammar (recursively)."
  for word in words:
    if word in grammar: rewrite(random.choice(grammar[word]), into)
    else: into.append(word)
  return into

def rewrite_tree(words, into):
  "Replace the list of words into a random tree, chosen from grammar."
  for word in words:
    if word in grammar:
      into.append({word: rewrite_tree(random.choice(grammar[word]), [])})
    else:
      into.append(word)
  return into

def generate(str='S'):
  "Replace each word in str by a random entry in grammar (recursively)."
  return ' '.join(rewrite(str.split(), []))

def generate_tree(cat='S'):
  "Use grammar to rewrite the category cat"
  return rewrite_tree([cat], [])
