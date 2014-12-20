//Copyright (C) 2013 Cem Bassoy.
//
//This file is part of the OpenCL Utility Toolkit.
//
//OpenCL Utility Toolkit is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//OpenCL Utility Toolkit is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with OpenCL Utility Toolkit.  If not, see <http://www.gnu.org/licenses/>.

#include <ocl_context.h>
#include <ocl_platform.h>
#include <ocl_sampler.h>
#include <ocl_query.h>
#include <utl_assert.h>

/*! \brief Instantiates this Sampler with the active Context.
  *
  * If no Context is provided the active Context of the Platform is taken if
  * there is an active Context.
  *
  * The OpenCL Sampler is not created. Do not forget to create the OpenCL Sampler.
  */
ocl::Sampler::Sampler () :
    _context(0), _id(0)
{
    if(!ocl::Platform::hasActivePlatform()) return;
    if(!ocl::Platform::activePlatform()->hasActiveContext()) return;

    _context = ocl::Platform::activePlatform()->activeContext();
    _context->insert(this);
}

/**
 * \brief Instantiates this Sampler for a given Context.
 *
 *
 * \param ctxt Context with which the Sampler is created.
 * \param amode AdressingMode specifies how out-of-range image coordinates are handled when reading from an image.
 * \param fmode FilterMode specifies the type of filter that must be applied when reading an image.
 * \param normalized determines if the image coordinates specified are normalized.
 */
ocl::Sampler::Sampler(Context& ctxt, AdressingMode amode, FilterMode fmode, bool normalized) : 
	_context(&ctxt), _id(0)
{
    this->create(amode, fmode, normalized);
}


/**
 * \brief ocl::Sampler::~Sampler.
 */
ocl::Sampler::~Sampler()
{
	this->release();
}

/**
 * \brief Creates the OpenCL Sampler with its Context.
 *
 * \param amode AdressingMode specifies how out-of-range image coordinates are handled when reading from an image.
 * \param fmode FilterMode specifies the type of filter that must be applied when reading an image.
 * \param normalized determines if the image coordinates specified are normalized.
 */
void ocl::Sampler::create(AdressingMode amode, FilterMode fmode, bool normalized)
{
    TRUE_ASSERT(this->_context != 0, "Context not valid - cannot create Sampler");
    
    cl_bool coords = normalized ? CL_TRUE : CL_FALSE;
    cl_addressing_mode cl_amode = amode;
    cl_filter_mode cl_fmode = fmode;
    cl_int status;
    
// #ifdef CL_VERSION_2_0
#ifdef OPENCL_V2_0
    cl_sampler_properties props[] = {
      CL_SAMPLER_NORMALIZED_COORDS, coords,
      CL_SAMPLER_ADDRESSING_MODE, cl_amode,
      CL_SAMPLER_FILTER_MODE, cl_fmode,
      0
    };
    
    this->_id = clCreateSamplerWithProperties( this->_context->id(), props, &status );
#else
    this->_id = clCreateSampler(this->_context->id(), coords, cl_amode, cl_fmode, &status);
#endif

    OPENCL_SAFE_CALL(status);
    TRUE_ASSERT(this->_id != 0, "Could not create Sampler.");
}

/**
 * \brief Returns pointer to Context of this Sampler.
 *
 * Pointer must not be NULL;
 */
const ocl::Context& ocl::Sampler::context() const
{
	TRUE_ASSERT(this->_context != 0, "Context not valid - cannot create Sampler");
	return *(this->_context);
}


/**
 * \brief Returns pointer to Context of this Sampler.
 *
 * Pointer must not be NULL;
 */
cl_sampler ocl::Sampler::id() const
{
	TRUE_ASSERT(this->_id != 0, "Could not release Sampler.");
	return this->_id;
}

/**
 * \brief Releases this Sampler.
 */
void ocl::Sampler::release()
{
	TRUE_ASSERT(this->_id != 0, "Could not release Sampler.");	
	cl_int status = clReleaseSampler (this->_id);
	OPENCL_SAFE_CALL(status);
}


/**
 * \brief Returns true if Sampler uses normalized coordinates.
 */
bool ocl::Sampler::isNormalized() const
{
	TRUE_ASSERT(this->_context != 0, "Context not valid - cannot create Sampler");
	TRUE_ASSERT(this->_id != 0, "Could not create Sampler.");
	
	cl_bool normalized;
	
	cl_int status = clGetSamplerInfo (this->_id, CL_SAMPLER_NORMALIZED_COORDS, sizeof(cl_bool), &normalized, NULL);
	OPENCL_SAFE_CALL(status);
	
	return normalized == CL_TRUE ? true : false;
}

/**
 * \brief Returns the addressing mode of this Sampler.
 *
 * \Return AdressingMode 
 */
ocl::Sampler::AdressingMode ocl::Sampler::addressingMode() const
{
	TRUE_ASSERT(this->_context != 0, "Context not valid - cannot create Sampler");
	TRUE_ASSERT(this->_id != 0, "Could not create Sampler.");
	
	cl_addressing_mode mode;
	
	cl_int status = clGetSamplerInfo (this->_id, CL_SAMPLER_ADDRESSING_MODE, sizeof(mode), &mode, NULL);
	OPENCL_SAFE_CALL(status);
	
	return AdressingMode(mode);
}

/**
 * \brief Returns the filtering mode of this Sampler.
 *
 * \Return FilterMode 
 */
ocl::Sampler::FilterMode ocl::Sampler::filterMode() const
{
	TRUE_ASSERT(this->_context != 0, "Context not valid - cannot create Sampler");
	TRUE_ASSERT(this->_id != 0, "Could not create Sampler.");
	
	cl_filter_mode mode;
	
	cl_int status = clGetSamplerInfo (this->_id, CL_SAMPLER_FILTER_MODE, sizeof(mode), &mode, NULL);
	OPENCL_SAFE_CALL(status);
	
	return FilterMode(mode);
}


