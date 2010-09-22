/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2008-2009  Marius Muja (mariusm@cs.ubc.ca). All rights reserved.
 * Copyright 2008-2009  David G. Lowe (lowe@cs.ubc.ca). All rights reserved.
 *
 * THE BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))



/* Nearest neighbour index algorithms */
enum flann_algorithm_t {
	LINEAR = 0,
	KDTREE = 1,
	KMEANS = 2,
	COMPOSITE = 3,
	SAVED = 254,
	AUTOTUNED = 255
};

enum flann_centers_init_t {
	CENTERS_RANDOM = 0,
	CENTERS_GONZALES = 1,
	CENTERS_KMEANSPP = 2
};

enum flann_log_level_t {
	LOG_NONE = 0,
	LOG_FATAL = 1,
	LOG_ERROR = 2,
	LOG_WARN = 3,
	LOG_INFO = 4
};

enum flann_distance_t {
	EUCLIDEAN = 1,
	MANHATTAN = 2,
	MINKOWSKI = 3
};

enum flann_datatype_t {
	INT8 = 0,
	INT16 = 1,
	INT32 = 2,
	INT64 = 3,
	UINT8 = 4,
	UINT16 = 5,
	UINT32 = 6,
	UINT64 = 7,
	FLOAT32 = 8,
	FLOAT64 = 9
};


struct FLANNParameters {
	enum flann_algorithm_t algorithm; /* the algorithm to use */

	/* search time parameters */
	int checks;                /* how many leafs (features) to check in one search */
    float cb_index;            /* cluster boundary index. Used when searching the kmeans tree */

    /*  kdtree index parameters */
    int trees;                 /* number of randomized trees to use (for kdtree) */

    /* kmeans index parameters */
	int branching;             /* branching factor (for kmeans tree) */
	int iterations;            /* max iterations to perform in one kmeans cluetering (kmeans tree) */
	enum flann_centers_init_t centers_init;  /* algorithm used for picking the initial cluetr centers for kmeans tree */

	/* autotuned index parameters */
	float target_precision;    /* precision desired (used for autotuning, -1 otherwise) */
	float build_weight;        /* build tree time weighting factor */
	float memory_weight;       /* index memory weigthing factor */
    float sample_fraction;     /* what fraction of the dataset to use for autotuning */

    /* other parameters */
    enum flann_log_level_t log_level;    /* determines the verbosity of each flann function */
	long random_seed;          		/* random seed to use */
};



#ifdef __cplusplus

#include <stdexcept>
#include <cassert>
#include "flann/util/object_factory.h"

namespace flann {


template <typename ELEM_TYPE>
struct DistType
{
	typedef ELEM_TYPE type;
};

template <>
struct DistType<unsigned char>
{
	typedef float type;
};

template <>
struct DistType<int>
{
	typedef float type;
};


class FLANNException : public std::runtime_error {
 public:
   FLANNException(const char* message) : std::runtime_error(message) { }

   FLANNException(const std::string& message) : std::runtime_error(message) { }
 };


struct IndexParams {
protected:
	IndexParams(flann_algorithm_t algorithm_) : algorithm(algorithm_) {};

public:
	static IndexParams* createFromParameters(const FLANNParameters& p);

	virtual flann_algorithm_t getIndexType() const = 0;

	virtual void fromParameters(const FLANNParameters& p) = 0;
	virtual void toParameters(FLANNParameters& p) const = 0;

	virtual void print() const = 0;

	flann_algorithm_t algorithm;
};


typedef ObjectFactory<IndexParams, flann_algorithm_t> ParamsFactory;


struct SearchParams {
	SearchParams(int checks_ = 32) :
		checks(checks_) {};

	int checks;
};

}

#endif

#endif  /* CONSTANTS_H */
