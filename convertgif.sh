ffmpeg -i drive_40_mph.mp4 -r 10 -f image2pipe -vcodec ppm - | \
  convert -delay 5 -loop 0 - output.gif
