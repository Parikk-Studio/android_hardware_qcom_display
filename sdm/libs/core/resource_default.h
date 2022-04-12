/*
* Copyright (c) 2014 - 2021, The Linux Foundation. All rights reserved.
*
* Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
*    * Redistributions of source code must retain the above copyright notice, this list of
*      conditions and the following disclaimer.
*    * Redistributions in binary form must reproduce the above copyright notice, this list of
*      conditions and the following disclaimer in the documentation and/or other materials provided
*      with the distribution.
*    * Neither the name of The Linux Foundation nor the names of its contributors may be used to
*      endorse or promote products derived from this software without specific prior written
*      permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
* Changes from Qualcomm Innovation Center are provided under the following license:
*
* Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
*    * Redistributions of source code must retain the above copyright
*      notice, this list of conditions and the following disclaimer.
*
*    * Redistributions in binary form must reproduce the above
*      copyright notice, this list of conditions and the following
*      disclaimer in the documentation and/or other materials provided
*      with the distribution.
*
*    * Neither the name of Qualcomm Innovation Center, Inc. nor the names of its
*      contributors may be used to endorse or promote products derived
*      from this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
* IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __RESOURCE_DEFAULT_H__
#define __RESOURCE_DEFAULT_H__

#include <core/display_interface.h>
#include <private/resource_interface.h>
#include <private/layer_feedback.h>
#include <utils/locker.h>
#include <private/hw_interface.h>
#include <vector>
#include <map>


namespace sdm {

class ResourceDefault : public ResourceInterface {
 public:
  static DisplayError CreateResourceDefault(const HWResourceInfo &hw_resource_info,
                                            ResourceInterface **resource_intf);
  static DisplayError DestroyResourceDefault(ResourceInterface *resource_intf);
  virtual DisplayError RegisterDisplay(int32_t display_id, DisplayType type,
                                       const HWDisplayAttributes &display_attributes,
                                       const HWPanelInfo &hw_panel_info,
                                       const HWMixerAttributes &mixer_attributes,
                                       const Resolution &fb_resolution,
                                       Handle *display_ctx);
  virtual DisplayError UnregisterDisplay(Handle display_ctx);
  virtual DisplayError ReconfigureDisplay(Handle display_ctx,
                                          const HWDisplayAttributes &display_attributes,
                                          const HWPanelInfo &hw_panel_info,
                                          const HWMixerAttributes &mixer_attributes,
                                          const Resolution &fb_resolution);
  virtual DisplayError Start(Handle display_ctx, LayerStack *layer_stack);
  virtual DisplayError Stop(Handle display_ctx, DispLayerStack *disp_layer_stack);
  virtual DisplayError SetDrawMethod(Handle display_ctx, const DisplayDrawMethod &draw_method);
  virtual DisplayError Prepare(Handle display_ctx, DispLayerStack *disp_layer_stack,
                               LayerFeedback *feedback);
  virtual DisplayError PostPrepare(Handle display_ctx, DispLayerStack *disp_layer_stack);
  virtual DisplayError Commit(Handle display_ctx, DispLayerStack *disp_layer_stack);
  virtual DisplayError PostCommit(Handle display_ctx, DispLayerStack *disp_layer_stack);
  virtual DisplayError Precheck(Handle display_ctx, DispLayerStack *disp_layer_stack,
                                LayerFeedback* feedback);
  virtual void Purge(Handle display_ctx);
  virtual DisplayError SetMaxMixerStages(Handle display_ctx, uint32_t max_mixer_stages);
  virtual DisplayError ValidateScaling(const LayerRect &crop, const LayerRect &dst, bool rotate90,
                                       BufferLayout layout, bool use_rotator_downscale);
  DisplayError ValidateCursorConfig(Handle display_ctx, const Layer *layer, bool is_top);
  DisplayError ValidateAndSetCursorPosition(Handle display_ctx, DispLayerStack *disp_layer_stack,
                                            int x, int y, DisplayConfigVariableInfo *fb_config);
  DisplayError SetMaxBandwidthMode(HWBwModes mode);
  virtual DisplayError SetDetailEnhancerData(Handle display_ctx,
                                             const DisplayDetailEnhancerData &de_data);
  virtual DisplayError UpdateSyncHandle(Handle display_ctx, const SyncPoints &sync_points);
  virtual DisplayError Perform(int cmd, ...) { return kErrorNone; }
  DisplayError SetDisplayState(int32_t display_id, DisplayState state) { return kErrorNone; }
  virtual bool IsRotatorSupportedFormat(LayerBufferFormat format) { return false; }
  virtual DisplayError FreeDemuraFetchResources(const int32_t &display_id) { return kErrorNone; }
  virtual DisplayError GetDemuraFetchResourceCount(
                       std::map<uint32_t, uint8_t> *fetch_resource_cnt) {
    return kErrorNone;
  }
  virtual DisplayError ReserveDemuraFetchResources(const int32_t &display_id,
                                                   const int8_t &preferred_rect) {
    return kErrorNone;
  }
  virtual DisplayError GetDemuraFetchResources(Handle display_ctx, FetchResourceList *frl) {
    return kErrorNone;
  }
  virtual DisplayError SetMaxSDEClk(Handle display_ctx, uint32_t clk) { return kErrorNotSupported; }
  virtual DisplayError ForceToneMapConfigure(Handle display_ctx, DispLayerStack *disp_layer_stack) {
    return kErrorNotSupported;
  }
  virtual DisplayError PreCommit(Handle display_ctx);
  virtual bool HandleCwbTeardown(Handle display_ctx) {
    return false;
  }
  virtual void HandleSkipValidate(Handle display_ctx);

 private:
  enum PipeOwner {
    kPipeOwnerUserMode,       // Pipe state when it is available for reservation
    kPipeOwnerKernelMode,  // Pipe state when pipe is owned by kernel
  };

  // todo: retrieve all these from kernel
  enum {
    kMaxDecimationDownScaleRatio = 16,
  };

  struct SourcePipe {
    PipeType type;
    PipeOwner owner;
    uint32_t mdss_pipe_id;
    uint32_t index;
    HWBlockType hw_block_type;
    int priority;

    SourcePipe()
      : type(kPipeTypeUnused),
        owner(kPipeOwnerUserMode),
        mdss_pipe_id(0),
        index(0),
        hw_block_type(kHWBlockMax),
        priority(0) {}

    inline void ResetState() { hw_block_type = kHWBlockMax; }
  };

  struct DisplayResourceContext {
    HWDisplayAttributes display_attributes;
    HWBlockType hw_block_type;
    uint64_t frame_count;
    HWMixerAttributes mixer_attributes;
    Resolution fb_resolution;

    DisplayResourceContext() : hw_block_type(kHWBlockMax), frame_count(0) {}
  };

  struct HWBlockContext {
    bool is_in_use;
    HWBlockContext() : is_in_use(false) { }
  };

  explicit ResourceDefault(const HWResourceInfo &hw_res_info);
  DisplayError Init();
  DisplayError Deinit();
  uint32_t NextPipe(PipeType pipe_type, HWBlockType hw_block_type);
  uint32_t SearchPipe(HWBlockType hw_block_type, SourcePipe *src_pipes, uint32_t num_pipe);
  uint32_t GetPipe(HWBlockType hw_block_type, bool need_scale);
  bool IsScalingNeeded(const HWPipeInfo *pipe_info);
  DisplayError Config(DisplayResourceContext *display_resource_ctx,
                      DispLayerStack *disp_layer_stack);
  DisplayError DisplaySplitConfig(DisplayResourceContext *display_resource_ctx,
                                 const LayerRect &src_rect, const LayerRect &dst_rect,
                                 HWLayerConfig *layer_config);
  DisplayError SrcSplitConfig(DisplayResourceContext *display_resource_ctx,
                             const LayerRect &src_rect, const LayerRect &dst_rect,
                             HWLayerConfig *layer_config);
  bool CalculateCropRects(const LayerRect &scissor, LayerRect *crop, LayerRect *dst);
  DisplayError ValidateLayerParams(const Layer *layer);
  DisplayError ValidateDimensions(const LayerRect &crop, const LayerRect &dst);
  DisplayError ValidatePipeParams(HWPipeInfo *pipe_info, LayerBufferFormat format);
  DisplayError ValidateDownScaling(float scale_x, float scale_y, bool ubwc_tiled);
  DisplayError ValidateUpScaling(float scale_x, float scale_y);
  DisplayError GetScaleFactor(const LayerRect &crop, const LayerRect &dst, float *scale_x,
                             float *scale_y);
  DisplayError SetDecimationFactor(HWPipeInfo *pipe);
  void SplitRect(const LayerRect &src_rect, const LayerRect &dst_rect, LayerRect *src_left,
                LayerRect *dst_left, LayerRect *src_right, LayerRect *dst_right);
  DisplayError AlignPipeConfig(const Layer *layer, HWPipeInfo *left_pipe,
                               HWPipeInfo *right_pipe);
  void ResourceStateLog(void);
  DisplayError CalculateDecimation(float downscale, uint8_t *decimation);
  DisplayError GetScaleLutConfig(HWScaleLutInfo *lut_info);

  HWResourceInfo hw_res_info_;
  HWBlockContext hw_block_ctx_[kHWBlockMax];
  std::vector<SourcePipe> src_pipes_;
  uint32_t num_pipe_ = 0;
};

}  // namespace sdm

#endif  // __RESOURCE_DEFAULT_H__

