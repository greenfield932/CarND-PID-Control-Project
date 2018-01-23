ffmpeg -y -i drive_max.mp4 -r 20 -c:v libx264 -b:v 3M -strict -2 -movflags faststart destination.mp4
