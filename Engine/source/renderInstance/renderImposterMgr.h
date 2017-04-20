//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _IMPOSTERRENDERMGR_H_
#define _IMPOSTERRENDERMGR_H_

#ifndef _RENDERBINMANAGER_H_
#include "renderInstance/renderBinManager.h"
#endif
#ifndef _GFXPRIMITIVEBUFFER_H_
#include "gfx/gfxPrimitiveBuffer.h"
#endif
#ifndef _MATTEXTURETARGET_H_
#include "materials/matTextureTarget.h"
#endif
#ifndef _TSLASTDETAIL_H_
#include "ts/tsLastDetail.h"
#endif

class TSLastDetail;
class GFXTextureObject;
class RenderDeferredMgr;
struct ImposterRenderInst;


/*
GFXDeclareVertexFormat( ImposterCorner )
{
   /// billboard corner index
   float corner;
};
*/

/// This is a special render manager for processing single 
/// billboard imposters typically generated by the tsLastDetail
/// class.  It tries to render them in large batches with as 
/// few state changes as possible.  For an example of use see 
/// TSLastDetail::render().
class RenderImposterMgr : public RenderBinManager
{
protected:
    
   typedef RenderBinManager Parent;

   static const U32 smImposterBatchSize = 1000;

   static U32 smRendered;
   static U32 smBatches;
   static U32 smDrawCalls;
   static U32 smPolyCount;
   static U32 smRTChanges;

   ImposterState mBuffer[smImposterBatchSize*4];
   
   GFXPrimitiveBufferHandle mIB;
   //GFXVertexBufferHandle<ImposterCorner> mCornerVB;   

   void _innerRender( const SceneRenderState *state, RenderDeferredMgr *prePassBin );

   void _renderDeferred( const SceneRenderState *state, RenderDeferredMgr *prePassBin, bool startDeferred );

   static bool _clearStats( GFXDevice::GFXDeviceEventType type );

public:

   static const RenderInstType RIT_Imposter;
   static const RenderInstType RIT_ImposterBatch;

   RenderImposterMgr( F32 renderOrder = 1.0f, F32 processAddOrder  = 1.0f );
   virtual ~RenderImposterMgr();

   // ConsoleObject
   DECLARE_CONOBJECT(RenderImposterMgr);
   static void initPersistFields();

   // RenderBinManager
   virtual void render( SceneRenderState *state );
};


/// This is a shared base render instance type TSLastDetail imposters.
/// @see TSLastDetail
/// @see RenderImposterMgr
struct ImposterBaseRenderInst : public RenderInst
{
   /// The material for this imposter.
   BaseMatInstance *mat;
};


/// This is a render instance for a single imposter.
struct ImposterRenderInst : public ImposterBaseRenderInst
{
   /// The imposter state.
   ImposterState state;

   /// Helper for setting this instance to a default state.
   void clear()
   {
      dMemset( this, 0, sizeof( ImposterRenderInst ) );
      type = RenderImposterMgr::RIT_Imposter;
   }
};


/// This is a render instance for a cached multiple imposter batch.
struct ImposterBatchRenderInst : public ImposterBaseRenderInst
{
   /// The pre-built vertex buffer batch of imposters.
   GFXVertexBufferHandleBase *vertBuff;

   /// Helper for setting this instance to a default state.
   void clear()
   {
      dMemset( this, 0, sizeof( ImposterBatchRenderInst ) );
      type = RenderImposterMgr::RIT_ImposterBatch;
   }
};

#endif // _TSIMPOSTERRENDERMGR_H_
