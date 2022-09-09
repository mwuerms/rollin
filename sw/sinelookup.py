import math

print("create a lookuptable for a sinewave")
ampl = pow(2,16)/2-1
offset = ampl
nb_steps = 48
angle_step = 360/nb_steps
lookup = []
for n in range(0, nb_steps):
    lookup.append(int(ampl * math.sin(2*math.pi*n*angle_step/360) + offset))
print(lookup)