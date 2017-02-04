#include <stdio.h>
#include <alsa/asoundlib.h>

static void error(const char *fmt,...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "nanomix: ");
	vfprintf(stderr, fmt, va);
	fprintf(stderr, "\n");
	va_end(va);
}

static snd_hctl_t *nanomix_open(unsigned int card)
{
    char hw[64];

    snprintf(hw, sizeof(hw), "hw:%u", card);
    
    snd_hctl_t *handle;
    int err = snd_hctl_open(&handle, hw, 0);
	if (err < 0)
    {
		error("nanomix_open %s open error: %s", card, snd_strerror(err));
        handle = NULL;
	}

    return handle;
}

static void nanomix_close(snd_hctl_t* handle)
{
    snd_hctl_close(handle);
}

static int nanomix_print_value(snd_hctl_t* handle, snd_hctl_elem_t *elem)
{
    (void)handle;

    int err;
	unsigned int item, idx, count, *tlv;

	snd_ctl_elem_id_t *id;
	snd_ctl_elem_info_t *info;
	snd_ctl_elem_value_t *value;
    snd_aes_iec958_t iec958;

	snd_ctl_elem_id_alloca(&id);
	snd_ctl_elem_info_alloca(&info);
    snd_ctl_elem_value_alloca(&value);

    {
		if ((err = snd_hctl_elem_info(elem, info)) < 0)
        {
			error("snd_hctl_elem_info error: %s\n", snd_strerror(err));
			return err;
		}
        
        unsigned int numid = snd_hctl_elem_get_numid(elem);
        snd_ctl_elem_type_t type = snd_ctl_elem_info_get_type(info);
        unsigned int count = snd_ctl_elem_info_get_count(info);
        const char* name = snd_hctl_elem_get_name(elem);

        printf("%u\t%-10s\t%u\t%-40s", numid,  snd_ctl_elem_type_name(type), count, name);

        if ((err = snd_hctl_elem_read(elem, value)) < 0)
        {
			error("snd_hctl_elem_read error: %s\n", snd_strerror(err));
			return err;
		}

        for (int idx = 0; idx < count; idx++)
        {
            switch (type)
            {
			case SND_CTL_ELEM_TYPE_BOOLEAN:
				printf("%s ", snd_ctl_elem_value_get_boolean(value, idx) ? "on" : "off");
				break;
			case SND_CTL_ELEM_TYPE_INTEGER:
				printf("%li ", snd_ctl_elem_value_get_integer(value, idx));
				break;
			case SND_CTL_ELEM_TYPE_INTEGER64:
				printf("%Li ", snd_ctl_elem_value_get_integer64(value, idx));
				break;
			case SND_CTL_ELEM_TYPE_ENUMERATED:
				printf("%u ", snd_ctl_elem_value_get_enumerated(value, idx));
				break;
			case SND_CTL_ELEM_TYPE_BYTES:
				printf("0x%02x ", snd_ctl_elem_value_get_byte(value, idx));
				break;
			case SND_CTL_ELEM_TYPE_IEC958:
				snd_ctl_elem_value_get_iec958(value, &iec958);
				printf("[AES0=0x%02x AES1=0x%02x AES2=0x%02x AES3=0x%02x] ",
				       iec958.status[0], iec958.status[1],
				       iec958.status[2], iec958.status[3]);
				break;
			default:
				printf("? ");
				break;
			}
		}
		printf("\n");
	}

	return 0;
}

static int nanomix_list_controls(snd_hctl_t* handle)
{
    int err;
    snd_hctl_elem_t *elem;
    
	if ((err = snd_hctl_load(handle)) < 0)
    {
		error("snd_hctl_load error: %s\n", snd_strerror(err));
		return err;
	}

	for (elem = snd_hctl_first_elem(handle); elem; elem = snd_hctl_elem_next(elem))
    {
        nanomix_print_value(handle, elem);
	}

    return 0;
}

static int nanomix_get_control(snd_hctl_t* handle, const char *control)
{
    int err;
    snd_hctl_elem_t *elem;
    
	if ((err = snd_hctl_load(handle)) < 0)
    {
		error("snd_hctl_load error: %s\n", snd_strerror(err));
		return err;
	}

	for (elem = snd_hctl_first_elem(handle); elem; elem = snd_hctl_elem_next(elem))
    {
        const char* name = snd_hctl_elem_get_name(elem);
        if (!strcmp(name, control))
        {
            nanomix_print_value(handle, elem);
        }
	}

    return 0;
}

static int nanomix_set_value(snd_hctl_t* handle, snd_hctl_elem_t *elem,
                             char **values, unsigned int num_values)
{
    //
    //
    //
    return 0;
}

static int nanomix_set_control(snd_hctl_t* handle, const char *control,
                               char **values, unsigned int num_values)
{
    int err;
    snd_hctl_elem_t *elem;
    
	if ((err = snd_hctl_load(handle)) < 0)
    {
		error("snd_hctl_load error: %s\n", snd_strerror(err));
		return err;
	}

	for (elem = snd_hctl_first_elem(handle); elem; elem = snd_hctl_elem_next(elem))
    {
        const char* name = snd_hctl_elem_get_name(elem);
        if (!strcmp(name, control))
        {
            nanomix_set_value(handle, elem, values, num_values);
        }
	}

    return 0;
}

int main(int argc, char **argv)
{
    int ret = 0;
    unsigned int card = 0;
    snd_hctl_t *handle = nanomix_open(card);
    if (!handle)
    {
        ret = 1; /* error */
        goto end;
    }

    if (argc == 1)
    {
        nanomix_list_controls(handle);
    }
    else if (argc == 2)
    {
        nanomix_get_control(handle, argv[1]);
    }
    else if (argc >= 3)
    {
        nanomix_set_control(handle, argv[1], &argv[2], argc - 2);
    }
    else
    {
        fprintf(stderr, "Usage: %s [control id] [value to set]\n", argv[0]);
        ret = 1; // error
    }
    
    nanomix_close(handle);

end:
    return ret;
}
