//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010-2013, Image Engine Design Inc. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Image Engine Design nor the names of any
//       other contributors to this software may be used to endorse or
//       promote products derived from this software without specific prior
//       written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#ifndef IECORE_CAPTURINGRENDERER_H
#define IECORE_CAPTURINGRENDERER_H

#include "IECore/Renderer.h"

namespace IECore
{

IE_CORE_FORWARDDECLARE( Primitive )
IE_CORE_FORWARDDECLARE( Group )

/// The CapturingRenderer doesn't render images at all, but instead turns what it is
/// given into IECore Groups and Primitives which can then be used for further processing.
/// Currently it doesn't support any calls before worldBegin(), as there is no IECore
/// class to represent an entire scene. The world generated by the renderer can be retrieved
/// as an IECore::Group using the world() method.
/// \ingroup renderingGroup
class CapturingRenderer : public Renderer
{

	public :

		IE_CORE_DECLARERUNTIMETYPED( CapturingRenderer, Renderer );

		CapturingRenderer();
		virtual ~CapturingRenderer();

		/// \par Implementation specific options :
		////////////////////////////////////////////////////////////
		///
		/// \li <b>"cp:objectFilter" StringVectorData []</b><br>
		/// Use this to specify a list of object filters. Procedurals and primitives
		/// where the "name" renderer attribute does not match one of the filters will
		/// be skipped. The "name" attribute is interpreted as a path, and if a path is
		/// requested, all parents of that path will be output as well. For example, if
		/// you set the option to [ "/root/leftArm/thumb" ], "/root", "/root/leftArm"
		/// and "/root/leftArm/thumb" will be output. Objects without a name will always
		/// be output. The filters also support wildcards, so you can specify things like
		/// "/root/wheel*Rim/bolt", "/root/torso/rib*", and "/root/*", the last of which
		/// will output "/root" and all its descendants.
		virtual void setOption( const std::string &name, ConstDataPtr value );
		
		virtual ConstDataPtr getOption( const std::string &name ) const;

		virtual void camera( const std::string &name, const CompoundDataMap &parameters );
		virtual void display( const std::string &name, const std::string &type, const std::string &data, const CompoundDataMap &parameters );

		virtual void worldBegin();
		virtual void worldEnd();

		virtual void transformBegin();
		virtual void transformEnd();
		virtual void setTransform( const Imath::M44f &m );
		virtual void setTransform( const std::string &coordinateSystem );
		virtual Imath::M44f getTransform() const;
		virtual Imath::M44f getTransform( const std::string &coordinateSystem ) const;
		virtual void concatTransform( const Imath::M44f &m );
		virtual void coordinateSystem( const std::string &name );

		virtual void attributeBegin();
		virtual void attributeEnd();
		/// \par Implementation specific procedural attributes :
		////////////////////////////////////////////////////////////
		///
		/// \li <b>"cp:procedural:reentrant" BoolData true</b><br>
		/// When true, procedurals may be evaluated in multiple parallel threads.
		/// When false they will be evaluated from the thread they were specified from.

		virtual void setAttribute( const std::string &name, ConstDataPtr value );
		virtual ConstDataPtr getAttribute( const std::string &name ) const;
		virtual void shader( const std::string &type, const std::string &name, const CompoundDataMap &parameters );
		virtual void light( const std::string &name, const std::string &handle, const CompoundDataMap &parameters );
		virtual void illuminate( const std::string &lightHandle, bool on );

		virtual void motionBegin( const std::set<float> &times );
		virtual void motionEnd();

		virtual void points( size_t numPoints, const PrimitiveVariableMap &primVars );
		virtual void disk( float radius, float z, float thetaMax, const PrimitiveVariableMap &primVars );
		virtual void curves( const CubicBasisf &basis, bool periodic, ConstIntVectorDataPtr numVertices, const IECore::PrimitiveVariableMap &primVars );
		virtual void text( const std::string &font, const std::string &text, float kerning = 1.0f, const PrimitiveVariableMap &primVars=PrimitiveVariableMap() );
		virtual void sphere( float radius, float zMin, float zMax, float thetaMax, const PrimitiveVariableMap &primVars );
		virtual void image( const Imath::Box2i &dataWindow, const Imath::Box2i &displayWindow, const PrimitiveVariableMap &primVars );
		virtual void mesh( ConstIntVectorDataPtr vertsPerFace, ConstIntVectorDataPtr vertIds, const std::string &interpolation, const PrimitiveVariableMap &primVars );
		virtual void nurbs( int uOrder, ConstFloatVectorDataPtr uKnot, float uMin, float uMax, int vOrder, ConstFloatVectorDataPtr vKnot, float vMin, float vMax, const PrimitiveVariableMap &primVars );
		virtual void patchMesh( const CubicBasisf &uBasis, const CubicBasisf &vBasis, int nu, bool uPeriodic, int nv, bool vPeriodic, const PrimitiveVariableMap &primVars );
		virtual void geometry( const std::string &type, const CompoundDataMap &topology, const PrimitiveVariableMap &primVars );
		
		virtual void procedural( ProceduralPtr proc );

		virtual void instanceBegin( const std::string &name, const CompoundDataMap &parameters );
		virtual void instanceEnd();
		virtual void instance( const std::string &name );

		virtual DataPtr command( const std::string &name, const CompoundDataMap &parameters );

		virtual void editBegin( const std::string &editType, const CompoundDataMap &parameters );
		virtual void editEnd();
		
		/// Returns the world that was captured.
		/// \todo If we had a class for representing whole Scenes (including stuff before worldBegin)
		/// then we could have a scene() method instead.
		ConstGroupPtr world();

	private :
		
		class Implementation;

		boost::shared_ptr<Implementation> m_implementation;

};

IE_CORE_DECLAREPTR( CapturingRenderer )

} // namespace IECore

#endif // IECORE_CAPTURINGRENDERER_H
