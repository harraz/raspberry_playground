import subprocess

# Command to capture a single frame
cmd = [
    'ffmpeg',
    '-f', 'video4linux2',
    '-i', '/dev/video0',
    '-vframes', '1',
    '-y',
    '/home/pi/playground1/ffmpeg_image.jpg'
]

# Run the command
subprocess.run(cmd)

print('Image captured and saved as ffmpeg_image.jpg')
