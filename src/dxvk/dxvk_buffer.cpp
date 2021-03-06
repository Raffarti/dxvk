#include "dxvk_buffer.h"
#include "dxvk_device.h"

namespace dxvk {
  
  DxvkBuffer::DxvkBuffer(
          DxvkDevice*           device,
    const DxvkBufferCreateInfo& createInfo,
          VkMemoryPropertyFlags memoryType)
  : m_device        (device),
    m_info          (createInfo),
    m_memFlags      (memoryType) {
    // Initialize a single backing bufer with one slice
    m_physBuffers[0] = this->allocPhysicalBuffer(1);
    m_physSlice      = this->allocPhysicalSlice();
  }
  
  
  void DxvkBuffer::rename(
    const DxvkPhysicalBufferSlice& slice) {
    m_physSlice = slice;
  }
  
  
  DxvkPhysicalBufferSlice DxvkBuffer::allocPhysicalSlice() {
    if (m_physSliceId >= m_physBuffers[m_physBufferId]->sliceCount()) {
      m_physBufferId = (m_physBufferId + 1) % m_physBuffers.size();
      m_physSliceId  = 0;
      
      if ((m_physBuffers[m_physBufferId] == nullptr)
       || (m_physBuffers[m_physBufferId]->sliceCount() < m_physSliceCount)) {
        // Make sure that all buffers have the same size. If we don't do this,
        // one of the physical buffers may grow indefinitely while the others
        // remain small, depending on the usage pattern of the application.
        m_physBuffers[m_physBufferId] = this->allocPhysicalBuffer(m_physSliceCount);
      } else if (m_physBuffers[m_physBufferId]->isInUse()) {
        // Allocate a new physical buffer if the current one is still in use.
        // This also indicates that the buffer gets updated frequently, so we
        // will double the size of the physical buffers to accomodate for it.
        if (m_physBufferId == 0)
          m_physSliceCount *= 2;
        
        m_physBuffers[m_physBufferId] = this->allocPhysicalBuffer(m_physSliceCount);
      }
    }
    
    return m_physBuffers[m_physBufferId]->slice(m_physSliceId++);
  }
  
  
  Rc<DxvkPhysicalBuffer> DxvkBuffer::allocPhysicalBuffer(VkDeviceSize sliceCount) const {
    TRACE(this, sliceCount);
    return m_device->allocPhysicalBuffer(m_info, sliceCount, m_memFlags);
  }
  
  
  DxvkBufferView::DxvkBufferView(
    const Rc<vk::DeviceFn>&         vkd,
    const Rc<DxvkBuffer>&           buffer,
    const DxvkBufferViewCreateInfo& info)
  : m_vkd(vkd), m_buffer(buffer), m_info(info) {
    this->createBufferView();
  }
  
  
  DxvkBufferView::~DxvkBufferView() {
    this->destroyBufferView();
  }
  
  
  void DxvkBufferView::createBufferView() {
    auto physicalSlice = this->slice();
    
    VkBufferViewCreateInfo viewInfo;
    viewInfo.sType  = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
    viewInfo.pNext  = nullptr;
    viewInfo.flags  = 0;
    viewInfo.buffer = physicalSlice.handle();
    viewInfo.format = m_info.format;
    viewInfo.offset = physicalSlice.offset();
    viewInfo.range  = physicalSlice.length();
    
    if (m_vkd->vkCreateBufferView(m_vkd->device(), &viewInfo, nullptr, &m_view) != VK_SUCCESS)
      throw DxvkError("DxvkBufferView::DxvkBufferView: Failed to create buffer view");
  }
  
  
  void DxvkBufferView::destroyBufferView() {
    m_vkd->vkDestroyBufferView(
      m_vkd->device(), m_view, nullptr);
  }
  
}