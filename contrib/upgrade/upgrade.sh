# Perl-script to upgrade from pre-beta versions
# Initial version by Colm Dougan, contributed by Thomas Nilsson
perl -pi -e 's/\bEnsure\s+(\w+)\s*\(\)/Ensure($1)/' $@
perl -pi -e 's/\bEnsure\s+(\w+)\s*\(void\)/Ensure($1)/' $@
perl -pi -e 's/\bfail\(\)/fail("expected exception")/' $@
perl -pi -e 's/\bexpect_call/expect/' $@
perl -pi -e 's/\bwant\((\w+), ([^)]+)/when($1, is_equal_to($2)/' $@
perl -pi -e 's/\bwill_respond\((\w+),\ ([^,\)]+)/expect($1, will_return($2)/' $@
perl -pi -e 's/\bwill_return\((\w+),\ ([^,\)]+)/expect($1, will_return($2)/' $@
perl -pi -e 's/\bwant_non_null_?\(\"(\w+)\"\)/when($1, is_non_null)/' $@
perl -pi -e 's/\bsetup\(/set_setup(/g' $@
perl -pi -e 's/\bteardown\(/set_teardown(/g' $@
perl -pi -e 's/\bexpect_never\(/never_expect(/g' $@
