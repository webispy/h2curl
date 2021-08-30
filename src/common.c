#include <curl/mprintf.h>

#include "common.h"

static void dump(const char *text, unsigned char *ptr, size_t size,
		 char nohex)
{
	size_t i;
	size_t c;
	unsigned int width = 0x10;

	if (nohex)
		/* without the hex output, we can fit more on screen */
		width = 0x40;

	fprintf(stderr, "%s, %lu bytes (0x%lx)\n", text,
		(unsigned long)size, (unsigned long)size);

	for (i = 0; i < size; i += width)
	{
		fprintf(stderr, "%4.4lx: ", (unsigned long)i);

		if (!nohex)
		{
			/* hex not disabled, show it */
			for (c = 0; c < width; c++)
				if (i + c < size)
					fprintf(stderr, "%02x ", ptr[i + c]);
				else
					fputs("   ", stderr);
		}

		for (c = 0; (c < width) && (i + c < size); c++)
		{
			/* check for 0D0A; if found, skip past and start a new line of output */
			if (nohex && (i + c + 1 < size) && ptr[i + c] == 0x0D &&
			    ptr[i + c + 1] == 0x0A)
			{
				i += (c + 2 - width);
				break;
			}
			fprintf(stderr, "%c",
				(ptr[i + c] >= 0x20) && (ptr[i + c] < 0x80) ? ptr[i + c] : '.');
			/* check again for 0D0A, to avoid an extra \n if it's at width */
			if (nohex && (i + c + 2 < size) &&
			    ptr[i + c + 1] == 0x0D && ptr[i + c + 2] == 0x0A)
			{
				i += (c + 3 - width);
				break;
			}
		}
		fputc('\n', stderr); /* newline */
	}
}

int my_trace(CURL *handle, curl_infotype type, char *data, size_t size,
	     void *userp)
{
	char timebuf[60];
	const char *text;
	struct input *i = (struct input *)userp;
	static time_t epoch_offset;
	static int known_offset;
	struct timeval tv;
	time_t secs;
	struct tm *now;
	(void)handle; /* prevent compiler warning */

	gettimeofday(&tv, NULL);
	if (!known_offset)
	{
		epoch_offset = time(NULL) - tv.tv_sec;
		known_offset = 1;
	}
	secs = epoch_offset + tv.tv_sec;
	now = localtime(&secs); /* not thread safe but we don't care */
	curl_msnprintf(timebuf, sizeof(timebuf), "%02d:%02d:%02d.%06ld",
		       now->tm_hour, now->tm_min, now->tm_sec,
		       (long)tv.tv_usec);

	switch (type)
	{
	case CURLINFO_TEXT:
		fprintf(stderr, "%s Info: %s", timebuf, data);
		/* FALLTHROUGH */
	default: /* in case a new one is introduced to shock us */
		return 0;

	case CURLINFO_HEADER_OUT:
		text = "=> Send header";
		dump(text, (unsigned char *)data, size, 1);
		break;
	case CURLINFO_DATA_OUT:
		text = "=> Send data";
		dump(text, (unsigned char *)data, size, 1);
		break;
	case CURLINFO_SSL_DATA_OUT:
		text = "=> Send SSL data";
		break;
	case CURLINFO_HEADER_IN:
		text = "<= Recv header";
		dump(text, (unsigned char *)data, size, 1);
		break;
	case CURLINFO_DATA_IN:
		text = "<= Recv data";
		dump(text, (unsigned char *)data, size, 1);
		break;
	case CURLINFO_SSL_DATA_IN:
		text = "<= Recv SSL data";
		break;
	}

	return 0;
}
