/* gopt.h   PUBILC DOMAIN 2015   t.gopt@purposeful.co.uk */

/* <http:///www.purposeful.co.uk/software/gopt> */

/*
  I, Tom Vajzovic, am the author of this software and its documentation.
  I permanently abandon all intellectual property rights in them, including
  copyright, trademarks, design rights, database right, patents, and the right
  to be identified as the author.

  I am fairly certain that the software does what the documentation says it
  does, but I do not guarantee that it does, or that it does what you think it
  should.  I do not guarantee that it will not have undesirable side effects.

  You are free to use, modify and distribute this software as you please, but
  you do so at your own risk.  If you do not pass on this warning then you may
  be responsible for any problems encountered by those who obtain the software
  through you.
*/

#ifndef GOPT_H_INCLUDED
#define GOPT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

struct option
{
  /* input: */
  char          short_name;
  const char   *long_name;

  /* input/output: */
  unsigned int  flags;

  /* output: */
  unsigned int  count;
  char         *argument;
};

/* values for flags: */
#define GOPT_ARGUMENT_OPTIONAL  0x000u /* option may or may not have an option-argument */
#define GOPT_ARGUMENT_FORBIDDEN 0x001u /* option must not have an option-argument */
#define GOPT_ARGUMENT_REQUIRED  0x002u /* option must have an option-argument */
#define GOPT_ARGUMENT_NO_HYPHEN 0x004u /* option-argument may not start with hyphen/minus */
#define GOPT_REPEATABLE         0x008u /* option may be specified more than once */
#define GOPT_SEEN_SHORT_WITHOUT 0x010u /* short form of option was present without an option-argument in argv */
#define GOPT_SEEN_SHORT_WITH    0x020u /* short form of option was present with an option-argument in argv */
#define GOPT_SEEN_LONG_WITHOUT  0x040u /* long form of option was present without an option-argument in argv */
#define GOPT_SEEN_LONG_WITH     0x080u /* long form of option was present with an option-argument in argv */
#define GOPT_LAST               0x100u /* this is the last element of the array */

int gopt (char **argv, struct option *options);
/*
  The function gopt() takes an argument vector argv, which has the same
  form as the second argument to main().  It removes from the vector all
  options and option-arguments, leaving only the program name, the
  operands and a null pointer at the end.  It returns the updated
  argument count argc.  It does not need to know the initial value of
  argc because the input vector must be null-terminated.

  It also takes an array of struct option, the elements of which are
  used for specifying the options which are to be recognized and for
  returning details of which options were present in the argument
  vector.

  The members of struct option are used as follows:

  On input, short_name is the single character that follows "-" to make
  a short option, or zero if there is no short form of this option.

  On input, long_name is the name of the long option excluding the "--",
  or NULL if there is no long form of this option.

  On return short_name and long_name are unaltered.

  count may be uninitialized on input.  On return it is set to the
  number of times the option was specified.  If the option is not
  present, this is zero.  If an option has both a short and long name,
  this is the sum of counts for both forms.

  argument may be uninitialized on input.  On return it is set to the
  last option-argument specified to this option, or NULL if no
  option-argument was specified with the last occurrence of this
  option, or the option was never specified.

  flags is the bitwise-or of certain constants.  On input flags must
  contain exactly one of:
  GOPT_ARGUMENT_FORBIDDEN if the option does not take an option-argument
  GOPT_ARGUMENT_REQUIRED if it always requires an option-argument
  GOPT_ARGUMENT_OPTIONAL if it may or may not take an option-argument

  If flags contains GOPT_ARGUMENT_REQUIRED, it may also contain
  GOPT_ARGUMENT_NO_HYPHEN.  If the option argument is not allowed to
  start with a '-', but the next member of the argument vector does
  start with one where an option argument was expected, then this flag
  causes it to be interpreted as another option, leaving the argument
  to the preceding option NULL.  The application can then detect the
  NULL and give an error message like "missing option argument" which
  may be preferable to treating the next program argument as an option
  argument and then giving a less helpful message when the option
  argument is found to be invalid.

  flags may also optionally contain GOPT_REPEATABLE.  This is ignored
  by the option parsing code, but may be used when checking the value
  returned in count, where if this flag is specified then the
  application will presumably report an error if count is greater than
  one.

  On return, the initial value of flags is still present, and
  additionally GOPT_SEEN_SHORT_WITH and/or GOPT_SEEN_SHORT_WITHOUT are
  set if the short name was used at least once with or without an
  option-argument, respectively, and GOPT_SEEN_LONG_WITH and/or
  GOPT_SEEN_LONG_WITHOUT are set if the long name was used.

  On input there must be zero or more elements in the options array
  initialized as above.  These must be followed by one element where
  flags is exactly GOPT_LAST.  All other members of this element may be
  uninitialized on input.  On return the count member of this element
  will be set to the total number of unrecognized options present.
  The short_name member will be set to the first unrecognized short
  option, or zero if there were none.  The long_name will be set to
  point to the start of the name of the first unrecognized long option,
  or NULL if there were none.  For completeness the argument member
  will be set to the option argument of the last unrecognized option
  (the same as with other options) but this is probably useless to the
  application.
*/

void gopt_errors (const char *argv0, const struct option *options);
/*
  The function gopt_errors() examines the array of struct option
  after it has been filled out by gopt().  If the options specified
  were invalid it prints an error message starting with argv0 and
  terminates the program.
*/

#ifdef __cplusplus
}
#endif

#endif /* GOPT_H_INCLUDED */
