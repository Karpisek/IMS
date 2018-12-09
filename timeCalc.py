import optparse

parser = optparse.OptionParser()

args = parser.parse_args()

file = open(args[1][0], "r")
lines = file.read().split('\n')
neprace = 0.0
prace = 0.0
delkaPrace = 0.0
while lines:
    if len(lines) > 2:
        prvni = lines[0].split(" ")
        druhy = lines[1].split(" ")
        if(prvni[1] == '1'):
            prace += float(druhy[0]) - float(prvni[0])
        else:
            neprace += float(druhy[0]) - float(prvni[0])
        lines.pop(0)
        lines.pop(0)
    else:
        prvni = lines[0].split(" ")
        delkaPrace = float(prvni[0])
        break

print("Delka prace: %.2f" % delkaPrace)
procento = delkaPrace /100
procentoPrace = prace/procento
procentoNeprace = neprace/procento
print("Cinny: %.2f %.2f%%"% (prace,  procentoPrace))
print("Necinny: %.2f %.2f%%"% (neprace, procentoNeprace))
