#pragma once

#include <math.h>
#include <IO.H>             // i/0 header definitions
#include <stdlib.h>         // standard library definitions
#include <WinDef.h>
#include <locale.h>

static HMODULE g_hLibIntl;

char * textdomain(const char * __domainname);
char * bindtextdomain(const char * __domainname, const char * __dirname);
char * chGetText(char * __msgid);

#define _(string) chGetText (string)
