/*
* mdrv_cryptlib.h- Sigmastar
*
* Copyright (C) 2018 Sigmastar Technology Corp.
*
* Author: edie.chen <edie.chen@sigmastar.com.tw>
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*/
#ifndef CRYPTLIB_H
# define CRYPTLIB_H

struct cipher_data {
	int init; /* 0 uninitialized */
	int blocksize;
	int aead;
	int stream;
	int ivsize;
	int alignmask;
	struct {
		/* block ciphers */
		struct crypto_ablkcipher *s;
		struct ablkcipher_request *request;
		struct cryptodev_result *result;
		uint8_t iv[EALG_MAX_BLOCK_LEN];
	} async;
};

int cryptodev_cipher_init(struct cipher_data *out, const char *alg_name,
			  uint8_t *key, size_t keylen, int stream, int aead);
void cryptodev_cipher_deinit(struct cipher_data *cdata);
int cryptodev_get_cipher_key(uint8_t *key, struct session_op *sop, int aead);
int cryptodev_get_cipher_keylen(unsigned int *keylen, struct session_op *sop,
		int aead);
ssize_t cryptodev_cipher_decrypt(struct cipher_data *cdata,
			const struct scatterlist *sg1,
			struct scatterlist *sg2, size_t len);
ssize_t cryptodev_cipher_encrypt(struct cipher_data *cdata,
				const struct scatterlist *sg1,
				struct scatterlist *sg2, size_t len);

static inline void cryptodev_cipher_set_iv(struct cipher_data *cdata,
				void *iv, size_t iv_size)
{
	memcpy(cdata->async.iv, iv, min(iv_size, sizeof(cdata->async.iv)));
}

static inline void cryptodev_cipher_get_iv(struct cipher_data *cdata,
				void *iv, size_t iv_size)
{
	memcpy(iv, cdata->async.iv, min(iv_size, sizeof(cdata->async.iv)));
}

/* Hash */
struct hash_data {
	int init; /* 0 uninitialized */
	int digestsize;
	int alignmask;
	struct {
		struct crypto_ahash *s;
		struct cryptodev_result *result;
		struct ahash_request *request;
	} async;
};

int cryptodev_hash_final(struct hash_data *hdata, void *output);
ssize_t cryptodev_hash_update(struct hash_data *hdata,
			struct scatterlist *sg, size_t len);
int cryptodev_hash_reset(struct hash_data *hdata);
void cryptodev_hash_deinit(struct hash_data *hdata);
int cryptodev_hash_init(struct hash_data *hdata, const char *alg_name,
			int hmac_mode, void *mackey, size_t mackeylen);


#endif
