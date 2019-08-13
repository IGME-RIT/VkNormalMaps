#include "Entity.h"

// The structure of data
// that is given to the
// uniform buffer
struct uniform_struct
{
	// combination of model, view, and 
	// projection matrices
	glm::mat4x4 projection;
	glm::mat4x4 view;
	glm::mat4x4 model;
};

Entity::Entity(Demo* demo)
{
	// make a createInfo for the buffer. It has the necessary sType, and it
	// has a usage bit that says this will be a Uniform Buffer. Throughout 
	// these tutorials we will be making many buffers that will be used
	// for many different things. We also set the size to the size of uniform_struct
	VkBufferCreateInfo buf_info = {};
	buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	buf_info.size = sizeof(uniform_struct);

	// we make a new CPU buffer, and we copy our data into the buffer.
	// we give memory_properties (which was created earlier) to help us
	// create the buffer
	matrixBufferCPU = new BufferCPU(demo->device, demo->memory_properties, buf_info);
}

Entity::~Entity()
{
	delete matrixBufferCPU;
}

void Entity::CreateDescriptorSetBasic(Demo* demo)
{
	// we need to allocate a space in memory for
	// our descriptor set. In this case, we will
	// only have one descriptor set. This set will
	// be stored in the descriptor pool that we made,
	// and this descriptor set will use the layout that
	// we made earlier (desc_layout) which is given to
	// the pipeline (explained later)
	VkDescriptorSetAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.descriptorPool = demo->desc_pool;
	alloc_info.descriptorSetCount = 1;
	alloc_info.pSetLayouts = &demo->desc_layout_basic;
	vkAllocateDescriptorSets(demo->device, &alloc_info, &descriptor_set);

	// The first descriptor will be the uniform buffer
	// because this descriptor is at binding #0 of the shader
	VkDescriptorBufferInfo buffer_info = {};
	buffer_info.range = sizeof(uniform_struct);
	buffer_info.buffer = matrixBufferCPU->buffer;

	// The next descriptor is the texture descriptor,
	// because this descriptor is at binding #1 of the shader
	VkDescriptorImageInfo texDesc = {};
	texDesc.sampler = demo->sampler;
	texDesc.imageView = texture[0]->textureGPU->imageView;
	texDesc.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	// VkWriteDescriptorSet does not a structure that allows
	// us to write to an entire set of descriptors,
	// it is a structure that allows us to write to one descriptor
	// that is part of a set

	// we need two of these, one for each descriptor in the set
	VkWriteDescriptorSet writes[2];

	// this initializes the bytes in the array to all be zero
	memset(&writes, 0, sizeof(writes));

	// sType is required
	// we are writing one descriptor per structure, so the 
	// descriptorCount per structure will be 1
	// dstSet is the destination set that we want to write to,
	// we only have one descriptor set (called "descriptor_set")
	// so that is the one we are using

	// The binding we are writing to is 0
	// The type of descriptor in this structure is a UNIFORM_BUFFER
	// and buffer_info is the VkDescriptorBufferInfo that we made
	// just a minute ago in this function
	writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[0].descriptorCount = 1;
	writes[0].dstSet = descriptor_set;
	writes[0].dstBinding = 0;
	writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writes[0].pBufferInfo = &buffer_info;

	// sType, descriptorCount, and dstSet is the same as before
	// The binding we are writing to is 1
	// The type of descriptor in this structure is a IMAGE_SAMPLER
	// and buffer_info is the VkDescriptorImageInfo that we made
	// just a minute ago in this function
	writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[1].descriptorCount = 1;
	writes[1].dstSet = descriptor_set;
	writes[1].dstBinding = 1;
	writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writes[1].pImageInfo = &texDesc;

	// update the descriptors, we give it the device (GPU),
	// we give it 2, because there are two elements in the
	// "writes" array, and we give it the "writes" array
	vkUpdateDescriptorSets(demo->device, 2, writes, 0, NULL);
}

void Entity::CreateDescriptorSetBumpy(Demo* demo)
{
	// we need to allocate a space in memory for
	// our descriptor set. In this case, we will
	// only have one descriptor set. This set will
	// be stored in the descriptor pool that we made,
	// and this descriptor set will use the layout that
	// we made earlier (desc_layout) which is given to
	// the pipeline (explained later)
	VkDescriptorSetAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.descriptorPool = demo->desc_pool;
	alloc_info.descriptorSetCount = 1;
	alloc_info.pSetLayouts = &demo->desc_layout_bumpy;
	vkAllocateDescriptorSets(demo->device, &alloc_info, &descriptor_set);

	// The first descriptor will be the uniform buffer
	// because this descriptor is at binding #0 of the shader
	VkDescriptorBufferInfo buffer_info = {};
	buffer_info.range = sizeof(uniform_struct);
	buffer_info.buffer = matrixBufferCPU->buffer;

	// The next descriptor is the texture descriptor,
	// because this descriptor is at binding #1 of the shader

	// We have two this time, one for color, one for normal
	VkDescriptorImageInfo texDesc[2] = {};
	for (int i = 0; i < 2; i++)
	{
		texDesc[i].sampler = demo->sampler;
		texDesc[i].imageView = texture[i]->textureGPU->imageView;
		texDesc[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	// VkWriteDescriptorSet does not a structure that allows
	// us to write to an entire set of descriptors,
	// it is a structure that allows us to write to one descriptor
	// that is part of a set

	// we need two of these, one for each descriptor in the set
	VkWriteDescriptorSet writes[3];

	// this initializes the bytes in the array to all be zero
	memset(&writes, 0, sizeof(writes));

	// sType is required
	// we are writing one descriptor per structure, so the 
	// descriptorCount per structure will be 1
	// dstSet is the destination set that we want to write to,
	// we only have one descriptor set (called "descriptor_set")
	// so that is the one we are using

	// The binding we are writing to is 0
	// The type of descriptor in this structure is a UNIFORM_BUFFER
	// and buffer_info is the VkDescriptorBufferInfo that we made
	// just a minute ago in this function
	writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[0].descriptorCount = 1;
	writes[0].dstSet = descriptor_set;
	writes[0].dstBinding = 0;
	writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writes[0].pBufferInfo = &buffer_info;

	// sType, descriptorCount, and dstSet is the same as before
	// The binding we are writing to is 1
	// The type of descriptor in this structure is a IMAGE_SAMPLER
	// and buffer_info is the VkDescriptorImageInfo that we made
	// just a minute ago in this function
	writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[1].descriptorCount = 1;
	writes[1].dstSet = descriptor_set;
	writes[1].dstBinding = 1;
	writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writes[1].pImageInfo = &texDesc[0];

	// Do it again for the normal texture
	writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[2].descriptorCount = 1;
	writes[2].dstSet = descriptor_set;
	writes[2].dstBinding = 2;
	writes[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writes[2].pImageInfo = &texDesc[1];

	// update the descriptors, we give it the device (GPU),
	// we give it 2, because there are two elements in the
	// "writes" array, and we give it the "writes" array
	vkUpdateDescriptorSets(demo->device, 3, writes, 0, NULL);
}

void Entity::Draw(VkCommandBuffer cmd, VkPipelineLayout pipeline_layout)
{
	// Bind our descriptor set to the GRAPHICS pipeline
	// Multiple pipelines of different types can be bound
	// to a command buffer at the same time
	vkCmdBindDescriptorSets(cmd,  VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &descriptor_set, 0, NULL);

	// Bind triangle vertex buffer
	// The offset is zero, which means we are starting with
	// the first vertex in the buffer. We are binding 1 buffer,
	// which is the GPU buffer, but this can be used to bind 
	// arrays of vertex buffers
	VkDeviceSize offsets[1] = { 0 };
	vkCmdBindVertexBuffers(cmd, 0, 1, &mesh->vertexDataGPU->buffer, offsets);

	// Bind triangle index buffer
	// We are using a 16-bit index buffer,
	// capping us to 65,000 vertices per buffer
	vkCmdBindIndexBuffer(cmd, mesh->indexDataGPU->buffer, 0, VK_INDEX_TYPE_UINT16);

	// Draw the indexed triangle
	// We have 36 indices in the index buffer
	// We are drawing these 36 indices one time
	vkCmdDrawIndexed(cmd, mesh->numIndices, 1, 0, 0, 1);
}

void Entity::Update(glm::mat4x4 projection, glm::mat4x4 view)
{
	// make temporary data where
	// we can store data that will be
	// in our buffer
	uniform_struct temporaryData;

	// create the MVP from the three matrices,
	// just like we did when we first made the buffer
	temporaryData.projection = projection;
	temporaryData.view = view;
	temporaryData.model = GetModelMatrix();

	// We store data into the buffer, just like
	// we did when we first made the buffer. We
	// do not need to destroy and rebuild the buffer,
	// we can reuse it
	matrixBufferCPU->Store(&temporaryData, sizeof(uniform_struct));
}

glm::mat4 Entity::GetModelMatrix()
{
	glm::mat4 model = parentModelMatrix;
	model = glm::translate(model, pos);
	model = glm::rotate(model, rot.y, glm::vec3{ 0.0f, 1.0f, 0.0f });
	model = glm::rotate(model, rot.x, glm::vec3{ 1.0f, 0.0f, 0.0f });
	model = glm::rotate(model, rot.z, glm::vec3{ 0.0f, 0.0f, 1.0f });
	model = glm::scale(model, scale);
	modelMatrix = model;

	return modelMatrix;
}

glm::vec3 Entity::GetWorldPosition()
{
	// This gets world position from model matrix
	// This works with parented and non-parented objects
	glm::mat4 t = this->GetModelMatrix();
	return glm::vec3(t[3]);

	/*
		Model Matrix

		  0 1 2  3
		[ x x x posX ]
		[ x x x posY ]
		[ x x x posZ ]
		[ x x x 1    ]

		glm::vec3(t[3]):
		{posX, posY, posZ}
	*/
}