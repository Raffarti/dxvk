#pragma once

#include "../dxvk_device.h"

#include "dxvk_hud_font.h"

namespace dxvk::hud {
  
  /**
   * \brief HUD coordinates
   * 
   * Coordinates relative to the top-left
   * corner of the swap image, in pixels.
   */
  struct HudPos {
    float x;
    float y;
  };
  
  /**
   * \brief Texture coordinates
   * 
   * Absolute texture coordinates that are used
   * to pick letters in the font texture.
   */
  struct HudTexCoord {
    uint16_t u;
    uint16_t v;
  };
  
  /**
   * \brief Color
   * 
   * SRGB color with alpha channel. The text
   * will use this color for the most part.
   */
  struct HudColor {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t w;
  };
  
  /**
   * \brief Text vertex
   */
  struct HudTextVertex {
    HudPos      position;
    HudTexCoord texcoord;
    HudColor    color;
  };
  
  /**
   * \brief Text renderer for the HUD
   * 
   * Can be used by the presentation backend to
   * display performance and driver information.
   */
  class HudTextRenderer {
    constexpr static VkDeviceSize MaxVertexCount = 1 << 16;
  public:
    
    HudTextRenderer(
      const Rc<DxvkDevice>&   device,
      const Rc<DxvkContext>&  context);
    
    ~HudTextRenderer();
    
    void beginFrame(
      const Rc<DxvkContext>&  context);
    
    void drawText(
      const Rc<DxvkContext>&  context,
            float             size,
            HudPos            pos,
            HudColor          color,
      const std::string&      text);
    
  private:
    
    std::array<uint8_t, 256> m_charMap;
    
    Rc<DxvkShader>      m_vertShader;
    Rc<DxvkShader>      m_fragShader;
    
    Rc<DxvkImage>       m_fontImage;
    Rc<DxvkImageView>   m_fontView;
    Rc<DxvkSampler>     m_fontSampler;
    
    Rc<DxvkBuffer>      m_vertexBuffer;
    size_t              m_vertexIndex = 0;
    
    Rc<DxvkShader> createVertexShader(
      const Rc<DxvkDevice>& device);
    
    Rc<DxvkShader> createFragmentShader(
      const Rc<DxvkDevice>& device);
    
    Rc<DxvkImage> createFontImage(
      const Rc<DxvkDevice>& device);
    
    Rc<DxvkImageView> createFontView(
      const Rc<DxvkDevice>& device);
    
    Rc<DxvkSampler> createFontSampler(
      const Rc<DxvkDevice>& device);
    
    Rc<DxvkBuffer> createVertexBuffer(
      const Rc<DxvkDevice>& device);
    
    void initFontTexture(
      const Rc<DxvkDevice>&  device,
      const Rc<DxvkContext>& context);
    
    void initCharMap();
    
  };
  
}