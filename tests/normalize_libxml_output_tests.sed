# 'time="?.?????"' => time="0.00000"
s/time=".+"/time="0.00000"/g
s/line=".+"/line="0"/g
# filenames, suites, libraries and "classnames" starting with "lib" or "cyg"
s/"lib/"/g
s/"cyg/"/g
