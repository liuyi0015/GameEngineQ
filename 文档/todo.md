待办；
关节动画？
3dRenderSystem软光栅
视频播放（FFmpeg）？
UI与相机无关
全局物体

相机的scale无效，只跟viewportWH有关
目前到viewTransform就直接画出了，没有视口变换
用了顶点绘制，但是顶点没有应用transform变换，而是直接对对象应用，再生成顶点