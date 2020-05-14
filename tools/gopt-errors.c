/* gopt-errors.c   PUBILC DOMAIN 2015   t.gopt@purposeful.co.uk */

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

#include <stdlib.h>
#include <stdio.h>

#ifdef _BSD_SOURCE
#include <sysexits.h>
#else
#define EX_USAGE EXIT_FAILURE
#endif

#include "gopt.h"

void gopt_errors (const char *argv0, const struct option *options)
{
  unsigned int bad_option_index = 0;
  unsigned int i;

  while (!(options[bad_option_index].flags & GOPT_LAST))
  {
    bad_option_index++;
  }

  if (options[bad_option_index].short_name)
  {
    fprintf (stderr, "%s: unrecognised option -%c\n", argv0, options[bad_option_index].short_name);
    exit (EX_USAGE);
  }

  if (options[bad_option_index].long_name)
  {
    fprintf (stderr, "%s: unrecognised option --%s\n", argv0, options[bad_option_index].long_name);
    exit (EX_USAGE);
  }

  for (i = 0; i < bad_option_index; i++)
  {
    if ((options[i].flags & GOPT_SEEN_SHORT_WITHOUT) && (options[i].flags & GOPT_ARGUMENT_REQUIRED))
    {
      fprintf (stderr, "%s: option -%c requires an option-argument\n", argv0, options[i].short_name);
      exit (EX_USAGE);
    }

    if ((options[i].flags & GOPT_SEEN_SHORT_WITH) && (options[i].flags & GOPT_ARGUMENT_FORBIDDEN))
    {
      fprintf (stderr, "%s: option -%c must not have an option-argument\n", argv0, options[i].short_name);
      exit (EX_USAGE);
    }

    if ((options[i].flags & GOPT_SEEN_LONG_WITHOUT) && (options[i].flags & GOPT_ARGUMENT_REQUIRED))
    {
      fprintf (stderr, "%s: option --%s requires an option-argument\n", argv0, options[i].long_name);
      exit (EX_USAGE);
    }

    if ((options[i].flags & GOPT_SEEN_LONG_WITH) && (options[i].flags & GOPT_ARGUMENT_FORBIDDEN))
    {
      fprintf (stderr, "%s: option --%s must not have an option-argument\n", argv0, options[i].long_name);
      exit (EX_USAGE);
    }

    if ((options[i].count > 1) && !(options[i].flags & GOPT_REPEATABLE))
    {
      fprintf (stderr, "%s: option -%c/--%s may not be repeated\n", argv0, options[i].short_name, options[i].long_name);
      exit (EX_USAGE);
    }
  }
}
