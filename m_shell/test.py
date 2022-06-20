import re

command = input("$+ ")
regex = re.compile('(.+)(&&|\|\|)(.+)')
game = re.findall(regex, command)
dir(game)