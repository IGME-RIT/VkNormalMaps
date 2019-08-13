To implement normal maps, we need a 3rd pipeline to handle more shaders,
we need a new type of Vertex, so that we can give Tangents to the shaders,
which will be used in the new pipeline. We also need a new descriptor set,
so that the shader can take 2 textures (color and normal), which will go into
the pipeline_layout of the 3rd pipeline. We will have 3 pipelines and 2 descriptor
sets. We also need to resize the descriptor pool to have size for all descriptors
that we want to use. Something important to know, normal mapping only
works on pixels that are being hit by pointlights, spotlights, or directional lights.
Pixels being touched by ambient occlusion (the blue light), will not have the
normal map effect.

There are two sets of textures that can be used with the building
rockColor + rockNormal
brickColor + brickNormal

Feel free to try both. The quality of the normal map effect is more noticable
when the window is larger, so feel free to increase the size