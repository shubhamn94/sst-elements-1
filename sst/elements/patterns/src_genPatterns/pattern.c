#include <stdio.h>
#include <stdlib.h>	/* For strtol() */
#include <string.h>
#include "main.h"
#include "pattern.h"



/* Local functions */
static void set_defaults(void);
static int error_check(void);


#define MAX_LINE		(1024)
#define MAX_PATTERN_NAME	(256)

/* These are the pattern names we recognize */
#define ALLREDUCE_NAME		"allreduce_pattern"
#define PINGPONG_NAME		"pingpong_pattern"
#define MSGRATE_NAME		"msgrate_pattern"

typedef enum {allreduce_pattern, pingpong_pattern, msgrate_pattern} pattern_t;


/* 
** Parameter variables. Locally global so I don't have to keep them
** passing back and forth (too many of them)
*/
static char _pattern_name[MAX_PATTERN_NAME];
pattern_t _pattern;


/* Allreduce parameters */
static int _num_ops;
static int _num_sets;

/* Pingpong parameters */
static int _destination;
static int _num_msgs;
static int _end_len;
static int _len_inc;

/* Msgrate parameters */
/* static int _num_msgs; already declared */
static int _msg_len;



static void
set_defaults(void)
{

    strcpy(_pattern_name, "");

    /* Allreduce defaults */
    _num_ops= NO_DEFAULT;
    _num_sets= NO_DEFAULT;

    /* Pingpong defaults */
    _destination= OPTIONAL;
    _num_msgs= NO_DEFAULT;
    _end_len= OPTIONAL;
    _len_inc= OPTIONAL;

    /* Msgrate defaults */
    _num_msgs= NO_DEFAULT;
    _msg_len= NO_DEFAULT;

}  /* end of set_defaults() */



#define PARAM_CHECK(p, x, c, v) \
    if (_##x c v)   { \
        error= TRUE; \
	fprintf(stderr, "%s parameter \"%s\" must be specified in input file\n", p, #x); \
    }

static int
error_check(void)
{

int error;


    error= FALSE;
    if (strcmp(_pattern_name, "") == 0)   {
	error= TRUE;
	fprintf(stderr, "Pattern name must be specified in input file\n");
    }

    switch (_pattern)   {
	case allreduce_pattern:
	    PARAM_CHECK("Allreduce", num_ops, <, 0);
	    PARAM_CHECK("Allreduce", num_sets, <, 0);
	    break;

	case pingpong_pattern:
	    /* Don't check optional parameter "destination" */
	    PARAM_CHECK("Pingpong", num_msgs, <, 0);
	    /* Don't check optional parameter "end_len" */
	    /* Don't check optional parameter "len_inc" */
	    break;

	case msgrate_pattern:
	    PARAM_CHECK("Msgrate", num_msgs, <, 0);
	    PARAM_CHECK("Msgrate", msg_len, <, 0);
	    break;
    }

    if (error)   {
	return FALSE;
    }

    /* No errors found */
    return TRUE;

}  /* end of error_check() */



void
disp_pattern_params(void)
{

    printf("*** Pattern name \"%s\"\n", pattern_name());
    switch (_pattern)   {
	case allreduce_pattern:
	    printf("***     num_sets =     %d\n", _num_sets);
	    printf("***     num_ops =      %d\n", _num_ops);
	    break;

	case pingpong_pattern:
	    if (_destination == OPTIONAL)   {
		printf("***     destination =  default\n");
	    } else   {
		printf("***     destination =  %d\n", _destination);
	    }
	    printf("***     num_msgs =     %d\n", _num_msgs);
	    if (_end_len == OPTIONAL)   {
		printf("***     end_len =      default\n");
	    } else   {
		printf("***     end_len =      %d\n", _end_len);
	    }
	    if (_len_inc == OPTIONAL)   {
		printf("***     len_inc =      default\n");
	    } else   {
		printf("***     len_inc =      %d\n", _len_inc);
	    }
	    break;

	case msgrate_pattern:
	    printf("***     num_msgs =     %d\n", _num_msgs);
	    printf("***     msg_len =      %d\n", _msg_len);
	    break;
    }

}  /* end of disp_pattern_params() */



int
read_pattern_file(FILE *fp_pattern, int verbose)
{

int error;
char line[MAX_LINE];
char *pos;
char key[MAX_LINE];
char value1[MAX_LINE];
int rc;


    if (fp_pattern == NULL)   {
	return FALSE;
    }

    /* Defaults */
    error= FALSE;
    set_defaults();


    /* Process the input file */
    while (fgets(line, MAX_LINE, fp_pattern) != NULL)   {
	/* Get rid of comments */
	pos= strchr(line, '#');
	if (pos)   {
	    *pos= '\0';
	}

	/* Now scan it */
	rc= sscanf(line, "%s = %s", key, value1);
	if (rc != 2)   {
	    rc= sscanf(line, "param %s = %s", key, value1);
	    if (rc != 2)   {
		fprintf(stderr, "Syntax error in this line:\n\t\"%s\"\nin pattern file!\n", line);
		return FALSE;
	    } else   {

		/* Process parameter entries */
		if (verbose > 1)   {
		    printf("Debug: Found parameter %s = %s\n", key, value1);
		}


		/* Allreduce parameters */
		if (strcmp("num_sets", key) == 0)   {
		    _num_sets= strtol(value1, (char **)NULL, 0);

		} else if (strcmp("num_ops", key) == 0)   {
		    _num_ops= strtol(value1, (char **)NULL, 0);


		/* Pingpong parameters */
		} else if (strcmp("destination", key) == 0)   {
		    _destination= strtol(value1, (char **)NULL, 0);

		} else if (strcmp("num_msgs", key) == 0)   {
		    _num_msgs= strtol(value1, (char **)NULL, 0);

		} else if (strcmp("end_len", key) == 0)   {
		    _end_len= strtol(value1, (char **)NULL, 0);

		} else if (strcmp("len_inc", key) == 0)   {
		    _len_inc= strtol(value1, (char **)NULL, 0);


		/* Msgrate parameters */
		} else if (strcmp("msg_len", key) == 0)   {
		    _msg_len= strtol(value1, (char **)NULL, 0);

		} else   {
		    fprintf(stderr, "Unknown parameter \"%s\" in pattern file!\n", key);
		    error= TRUE;
		}
	    }
	} else   {

	    /* Process name entries */
	    if (verbose > 1)   {
		printf("Debug: Found %s = %s\n", key, value1);
	    }

	    if (strcmp("name", key) == 0)   {
		strncpy(_pattern_name, value1, MAX_PATTERN_NAME);
		if (strcmp(_pattern_name, ALLREDUCE_NAME) == 0)   {
		    _pattern= allreduce_pattern;
		} else if (strcmp(_pattern_name, PINGPONG_NAME) == 0)   {
		    _pattern= pingpong_pattern;
		} else if (strcmp(_pattern_name, MSGRATE_NAME) == 0)   {
		    _pattern= msgrate_pattern;
		} else   {
		    fprintf(stderr, "Unknown pattern name \"%s\" in pattern file!\n", _pattern_name);
		    error= TRUE;
		}

	    } else   {
		fprintf(stderr, "Unknown entry \"%s\" in pattern file!\n", key);
		error= TRUE;
	    }
	}
    }

    if (error)   {
	return FALSE;
    }

    if (error_check() == FALSE)   {
	return FALSE;
    }

    /* Seems OK */
    return TRUE;

}  /* end of read_pattern_file() */



char *
pattern_name(void)
{
    return _pattern_name;
}  /* end of pattern_name() */



#define PRINT_PARAM(f, p) \
    fprintf(f, "    <%s> %d </%s>\n", #p, _##p, #p);

void
pattern_params(FILE *out)
{

    switch (_pattern)   {
	case allreduce_pattern:
	    PRINT_PARAM(out, num_sets);
	    PRINT_PARAM(out, num_ops);
	    break;

	case pingpong_pattern:
	    if (_destination != OPTIONAL)   {
		PRINT_PARAM(out, destination);
	    }
	    PRINT_PARAM(out, num_msgs);
	    if (_end_len != OPTIONAL)   {
		PRINT_PARAM(out, end_len);
	    }
	    if (_len_inc != OPTIONAL)   {
		PRINT_PARAM(out, len_inc);
	    }
	    break;

	case msgrate_pattern:
	    PRINT_PARAM(out, num_msgs);
	    PRINT_PARAM(out, msg_len);
	    break;
    }

}  /* end of pattern_params() */
