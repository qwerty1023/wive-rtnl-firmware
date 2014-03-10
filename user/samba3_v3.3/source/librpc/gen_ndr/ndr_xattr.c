/* parser auto-generated by pidl */

#include "includes.h"
#include "librpc/gen_ndr/ndr_xattr.h"

_PUBLIC_ enum ndr_err_code ndr_push_tdb_xattr(struct ndr_push *ndr, int ndr_flags, const struct tdb_xattr *r)
{
	if (ndr_flags & NDR_SCALARS) {
		NDR_CHECK(ndr_push_align(ndr, 4));
		{
			uint32_t _flags_save_string = ndr->flags;
			ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
			NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->name));
			ndr->flags = _flags_save_string;
		}
		NDR_CHECK(ndr_push_DATA_BLOB(ndr, NDR_SCALARS, r->value));
	}
	if (ndr_flags & NDR_BUFFERS) {
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_tdb_xattr(struct ndr_pull *ndr, int ndr_flags, struct tdb_xattr *r)
{
	if (ndr_flags & NDR_SCALARS) {
		NDR_CHECK(ndr_pull_align(ndr, 4));
		{
			uint32_t _flags_save_string = ndr->flags;
			ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
			NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->name));
			ndr->flags = _flags_save_string;
		}
		NDR_CHECK(ndr_pull_DATA_BLOB(ndr, NDR_SCALARS, &r->value));
	}
	if (ndr_flags & NDR_BUFFERS) {
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_tdb_xattr(struct ndr_print *ndr, const char *name, const struct tdb_xattr *r)
{
	ndr_print_struct(ndr, name, "tdb_xattr");
	ndr->depth++;
	ndr_print_string(ndr, "name", r->name);
	ndr_print_DATA_BLOB(ndr, "value", r->value);
	ndr->depth--;
}

_PUBLIC_ enum ndr_err_code ndr_push_tdb_xattrs(struct ndr_push *ndr, int ndr_flags, const struct tdb_xattrs *r)
{
	uint32_t cntr_xattrs_0;
	if (ndr_flags & NDR_SCALARS) {
		NDR_CHECK(ndr_push_align(ndr, 4));
		NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->num_xattrs));
		for (cntr_xattrs_0 = 0; cntr_xattrs_0 < r->num_xattrs; cntr_xattrs_0++) {
			NDR_CHECK(ndr_push_tdb_xattr(ndr, NDR_SCALARS, &r->xattrs[cntr_xattrs_0]));
		}
	}
	if (ndr_flags & NDR_BUFFERS) {
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_tdb_xattrs(struct ndr_pull *ndr, int ndr_flags, struct tdb_xattrs *r)
{
	uint32_t size_xattrs_0 = 0;
	uint32_t cntr_xattrs_0;
	TALLOC_CTX *_mem_save_xattrs_0;
	if (ndr_flags & NDR_SCALARS) {
		NDR_CHECK(ndr_pull_align(ndr, 4));
		NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->num_xattrs));
		size_xattrs_0 = r->num_xattrs;
		NDR_PULL_ALLOC_N(ndr, r->xattrs, size_xattrs_0);
		_mem_save_xattrs_0 = NDR_PULL_GET_MEM_CTX(ndr);
		NDR_PULL_SET_MEM_CTX(ndr, r->xattrs, 0);
		for (cntr_xattrs_0 = 0; cntr_xattrs_0 < size_xattrs_0; cntr_xattrs_0++) {
			NDR_CHECK(ndr_pull_tdb_xattr(ndr, NDR_SCALARS, &r->xattrs[cntr_xattrs_0]));
		}
		NDR_PULL_SET_MEM_CTX(ndr, _mem_save_xattrs_0, 0);
	}
	if (ndr_flags & NDR_BUFFERS) {
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_tdb_xattrs(struct ndr_print *ndr, const char *name, const struct tdb_xattrs *r)
{
	uint32_t cntr_xattrs_0;
	ndr_print_struct(ndr, name, "tdb_xattrs");
	ndr->depth++;
	ndr_print_uint32(ndr, "num_xattrs", r->num_xattrs);
	ndr->print(ndr, "%s: ARRAY(%d)", "xattrs", (int)r->num_xattrs);
	ndr->depth++;
	for (cntr_xattrs_0=0;cntr_xattrs_0<r->num_xattrs;cntr_xattrs_0++) {
		char *idx_0=NULL;
		if (asprintf(&idx_0, "[%d]", cntr_xattrs_0) != -1) {
			ndr_print_tdb_xattr(ndr, "xattrs", &r->xattrs[cntr_xattrs_0]);
			free(idx_0);
		}
	}
	ndr->depth--;
	ndr->depth--;
}

_PUBLIC_ enum ndr_err_code ndr_push_security_descriptor_hash(struct ndr_push *ndr, int ndr_flags, const struct security_descriptor_hash *r)
{
	if (ndr_flags & NDR_SCALARS) {
		NDR_CHECK(ndr_push_align(ndr, 4));
		NDR_CHECK(ndr_push_unique_ptr(ndr, r->sd));
		NDR_CHECK(ndr_push_array_uint8(ndr, NDR_SCALARS, r->hash, 16));
	}
	if (ndr_flags & NDR_BUFFERS) {
		if (r->sd) {
			NDR_CHECK(ndr_push_security_descriptor(ndr, NDR_SCALARS|NDR_BUFFERS, r->sd));
		}
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_security_descriptor_hash(struct ndr_pull *ndr, int ndr_flags, struct security_descriptor_hash *r)
{
	uint32_t _ptr_sd;
	TALLOC_CTX *_mem_save_sd_0;
	uint32_t size_hash_0 = 0;
	if (ndr_flags & NDR_SCALARS) {
		NDR_CHECK(ndr_pull_align(ndr, 4));
		NDR_CHECK(ndr_pull_generic_ptr(ndr, &_ptr_sd));
		if (_ptr_sd) {
			NDR_PULL_ALLOC(ndr, r->sd);
		} else {
			r->sd = NULL;
		}
		size_hash_0 = 16;
		NDR_CHECK(ndr_pull_array_uint8(ndr, NDR_SCALARS, r->hash, size_hash_0));
	}
	if (ndr_flags & NDR_BUFFERS) {
		if (r->sd) {
			_mem_save_sd_0 = NDR_PULL_GET_MEM_CTX(ndr);
			NDR_PULL_SET_MEM_CTX(ndr, r->sd, 0);
			NDR_CHECK(ndr_pull_security_descriptor(ndr, NDR_SCALARS|NDR_BUFFERS, r->sd));
			NDR_PULL_SET_MEM_CTX(ndr, _mem_save_sd_0, 0);
		}
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_security_descriptor_hash(struct ndr_print *ndr, const char *name, const struct security_descriptor_hash *r)
{
	ndr_print_struct(ndr, name, "security_descriptor_hash");
	ndr->depth++;
	ndr_print_ptr(ndr, "sd", r->sd);
	ndr->depth++;
	if (r->sd) {
		ndr_print_security_descriptor(ndr, "sd", r->sd);
	}
	ndr->depth--;
	ndr_print_array_uint8(ndr, "hash", r->hash, 16);
	ndr->depth--;
}

static enum ndr_err_code ndr_push_xattr_NTACL_Info(struct ndr_push *ndr, int ndr_flags, const union xattr_NTACL_Info *r)
{
	if (ndr_flags & NDR_SCALARS) {
		int level = ndr_push_get_switch_value(ndr, r);
		NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, level));
		switch (level) {
			case 1: {
				NDR_CHECK(ndr_push_unique_ptr(ndr, r->sd));
			break; }

			case 2: {
				NDR_CHECK(ndr_push_unique_ptr(ndr, r->sd_hs));
			break; }

			default:
				return ndr_push_error(ndr, NDR_ERR_BAD_SWITCH, "Bad switch value %u", level);
		}
	}
	if (ndr_flags & NDR_BUFFERS) {
		int level = ndr_push_get_switch_value(ndr, r);
		switch (level) {
			case 1:
				if (r->sd) {
					NDR_CHECK(ndr_push_security_descriptor(ndr, NDR_SCALARS|NDR_BUFFERS, r->sd));
				}
			break;

			case 2:
				if (r->sd_hs) {
					NDR_CHECK(ndr_push_security_descriptor_hash(ndr, NDR_SCALARS|NDR_BUFFERS, r->sd_hs));
				}
			break;

			default:
				return ndr_push_error(ndr, NDR_ERR_BAD_SWITCH, "Bad switch value %u", level);
		}
	}
	return NDR_ERR_SUCCESS;
}

static enum ndr_err_code ndr_pull_xattr_NTACL_Info(struct ndr_pull *ndr, int ndr_flags, union xattr_NTACL_Info *r)
{
	int level;
	uint16_t _level;
	TALLOC_CTX *_mem_save_sd_0;
	uint32_t _ptr_sd;
	TALLOC_CTX *_mem_save_sd_hs_0;
	uint32_t _ptr_sd_hs;
	level = ndr_pull_get_switch_value(ndr, r);
	if (ndr_flags & NDR_SCALARS) {
		NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &_level));
		if (_level != level) {
			return ndr_pull_error(ndr, NDR_ERR_BAD_SWITCH, "Bad switch value %u for r", _level);
		}
		switch (level) {
			case 1: {
				NDR_CHECK(ndr_pull_generic_ptr(ndr, &_ptr_sd));
				if (_ptr_sd) {
					NDR_PULL_ALLOC(ndr, r->sd);
				} else {
					r->sd = NULL;
				}
			break; }

			case 2: {
				NDR_CHECK(ndr_pull_generic_ptr(ndr, &_ptr_sd_hs));
				if (_ptr_sd_hs) {
					NDR_PULL_ALLOC(ndr, r->sd_hs);
				} else {
					r->sd_hs = NULL;
				}
			break; }

			default:
				return ndr_pull_error(ndr, NDR_ERR_BAD_SWITCH, "Bad switch value %u", level);
		}
	}
	if (ndr_flags & NDR_BUFFERS) {
		switch (level) {
			case 1:
				if (r->sd) {
					_mem_save_sd_0 = NDR_PULL_GET_MEM_CTX(ndr);
					NDR_PULL_SET_MEM_CTX(ndr, r->sd, 0);
					NDR_CHECK(ndr_pull_security_descriptor(ndr, NDR_SCALARS|NDR_BUFFERS, r->sd));
					NDR_PULL_SET_MEM_CTX(ndr, _mem_save_sd_0, 0);
				}
			break;

			case 2:
				if (r->sd_hs) {
					_mem_save_sd_hs_0 = NDR_PULL_GET_MEM_CTX(ndr);
					NDR_PULL_SET_MEM_CTX(ndr, r->sd_hs, 0);
					NDR_CHECK(ndr_pull_security_descriptor_hash(ndr, NDR_SCALARS|NDR_BUFFERS, r->sd_hs));
					NDR_PULL_SET_MEM_CTX(ndr, _mem_save_sd_hs_0, 0);
				}
			break;

			default:
				return ndr_pull_error(ndr, NDR_ERR_BAD_SWITCH, "Bad switch value %u", level);
		}
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_xattr_NTACL_Info(struct ndr_print *ndr, const char *name, const union xattr_NTACL_Info *r)
{
	int level;
	level = ndr_print_get_switch_value(ndr, r);
	ndr_print_union(ndr, name, level, "xattr_NTACL_Info");
	switch (level) {
		case 1:
			ndr_print_ptr(ndr, "sd", r->sd);
			ndr->depth++;
			if (r->sd) {
				ndr_print_security_descriptor(ndr, "sd", r->sd);
			}
			ndr->depth--;
		break;

		case 2:
			ndr_print_ptr(ndr, "sd_hs", r->sd_hs);
			ndr->depth++;
			if (r->sd_hs) {
				ndr_print_security_descriptor_hash(ndr, "sd_hs", r->sd_hs);
			}
			ndr->depth--;
		break;

		default:
			ndr_print_bad_level(ndr, name, level);
	}
}

_PUBLIC_ enum ndr_err_code ndr_push_xattr_NTACL(struct ndr_push *ndr, int ndr_flags, const struct xattr_NTACL *r)
{
	if (ndr_flags & NDR_SCALARS) {
		NDR_CHECK(ndr_push_align(ndr, 4));
		NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->version));
		NDR_CHECK(ndr_push_set_switch_value(ndr, &r->info, r->version));
		NDR_CHECK(ndr_push_xattr_NTACL_Info(ndr, NDR_SCALARS, &r->info));
	}
	if (ndr_flags & NDR_BUFFERS) {
		NDR_CHECK(ndr_push_xattr_NTACL_Info(ndr, NDR_BUFFERS, &r->info));
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_xattr_NTACL(struct ndr_pull *ndr, int ndr_flags, struct xattr_NTACL *r)
{
	if (ndr_flags & NDR_SCALARS) {
		NDR_CHECK(ndr_pull_align(ndr, 4));
		NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->version));
		NDR_CHECK(ndr_pull_set_switch_value(ndr, &r->info, r->version));
		NDR_CHECK(ndr_pull_xattr_NTACL_Info(ndr, NDR_SCALARS, &r->info));
	}
	if (ndr_flags & NDR_BUFFERS) {
		NDR_CHECK(ndr_pull_xattr_NTACL_Info(ndr, NDR_BUFFERS, &r->info));
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_xattr_NTACL(struct ndr_print *ndr, const char *name, const struct xattr_NTACL *r)
{
	ndr_print_struct(ndr, name, "xattr_NTACL");
	ndr->depth++;
	ndr_print_uint16(ndr, "version", r->version);
	ndr_print_set_switch_value(ndr, &r->info, r->version);
	ndr_print_xattr_NTACL_Info(ndr, "info", &r->info);
	ndr->depth--;
}
