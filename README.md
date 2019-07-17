PCG
===========

Attempt to make a procedural terrain generator. Using a random subdivision algorithm to subdivide tiles. Location of the
tiles is used to seed the random function.

There's some code to make biomes which would then have there own parameters for the algorithm. There's a bug in the code
that tries to detect biome edges. Edge detection is important as the generator needs to smoothly transition from one
biome to another.

Lighting is kind of mess right now. I remember it being better but maybe it was always a mess?
