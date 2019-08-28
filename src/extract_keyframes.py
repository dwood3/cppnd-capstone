import subprocess
import sys

def get_frame_types(video_fn):
    command = 'ffprobe -v error -threads auto -show_entries frame=pict_type -of default=noprint_wrappers=1'.split()
    out = subprocess.check_output(command + [video_fn]).decode()
    frame_types = out.replace('pict_type=','').split()
    return zip(range(len(frame_types)), frame_types)

def save_i_keyframes(video_fn):
    frame_types = get_frame_types(video_fn)
    i_frames = [x[0] for x in frame_types if x[1]=='I']

    with open('keyframes.txt', 'w') as file:
        file.write("%i\n" % len(i_frames))
        for frame_no in i_frames:
            file.write("%i\n" % frame_no)

if __name__ == '__main__':
    save_i_keyframes(sys.argv[1])
