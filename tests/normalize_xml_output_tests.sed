# 'time="?.?????"' => time="0.00000"
s/time=".+"/time="0.00000"/g
s/line=".+"/line="0"/g
# filenames, suites, libraries and "classnames" starting with "lib", "cyg" or ...
s/"lib/"/g
s/"cyg/"/g
s/"msys-/"/g
