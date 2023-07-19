#ifndef __CFG80211_H__
#define __CFG80211_H__

#include <net/cfg80211.h>
#include <fmac_api.h>

#define WIPHY_NAME "nrf_wifi"
#define NDEV_NAME "nrf_wifi%d"

enum nrf_vif_status {
	NRF_VIF_IDLE,
	NRF_VIF_CONNECTING,
	NRF_VIF_CONNECTED
};

struct ndev_priv_context {
	struct nrf_vif_priv *nrf_vif;
	struct wireless_dev wdev;
};

struct nrf_vif_priv {
	struct wiphy *wiphy;
	struct net_device *ndev;
	struct wifi_nrf_rpu_priv_lnx *rpu_priv;

	struct work_struct ws_scan;
	struct work_struct ws_connected;
	struct cfg80211_scan_request *scan_request;
	struct nrf_wifi_umac_auth_info auth_info;
	struct nrf_wifi_umac_assoc_info assoc_info;
	struct cfg80211_bss *bss;
#ifdef CONFIG_NRF700X_DATA_TX
	void *data_txq;
	struct work_struct ws_data_tx;
	struct work_struct ws_queue_monitor;
	unsigned long long num_tx_pkt;
#endif
	unsigned char vif_idx;
	enum nrf_vif_status vif_status;
	u8 mac_addr[ETH_ALEN];
};

struct wifi_nrf_rpu_priv_lnx *nrf_cfg80211_init(struct device *dev);

void nrf_cfg80211_uninit(struct wifi_nrf_rpu_priv_lnx *rpu_priv);

#ifdef CONFIG_NRF700X_DATA_TX
enum wifi_nrf_status wifi_nrf_if_carr_state_chg(void *vif_ctx,
						enum wifi_nrf_fmac_if_carr_state carr_state);

void wifi_nrf_wpa_supp_event_get_wiphy_lnx(void *if_priv,
					   struct nrf_wifi_event_get_wiphy *wiphy_info,
					   unsigned int event_len);

enum wifi_nrf_status wifi_nrf_if_carr_state_chg(void *vif_ctx,
						enum wifi_nrf_fmac_if_carr_state carr_state);

void nrf_wifi_umac_event_rx_frm_lnx(void *os_vif_ctx, void *frm);
#endif

#ifdef CONFIG_WPA_SUPP
void wifi_nrf_wpa_supp_event_proc_scan_start_lnx(void *os_vif_ctx,
						 struct nrf_wifi_umac_event_trigger_scan *scan_start_event,
						 unsigned int event_len);

void wifi_nrf_wpa_supp_event_proc_scan_done_lnx(void *os_vif_ctx,
			struct nrf_wifi_umac_event_trigger_scan *scan_done_event,
			unsigned int event_len);

void wifi_nrf_wpa_supp_event_proc_scan_res_lnx(void *os_vif_ctx,
					       struct nrf_wifi_umac_event_new_scan_results *scan_res,
					       unsigned int event_len,
					       bool more_res);

void wifi_nrf_wpa_supp_event_proc_disassoc_lnx(void *if_priv,
					       struct nrf_wifi_umac_event_mlme *disassoc,
					       unsigned int event_len);

void wifi_nrf_wpa_supp_event_proc_auth_resp_lnx(void *vif_ctx,
						struct nrf_wifi_umac_event_mlme *auth_resp,
						unsigned int event_len);

void wifi_nrf_wpa_supp_event_proc_assoc_resp_lnx(void *vif_ctx,
						 struct nrf_wifi_umac_event_mlme *assoc_resp,
						 unsigned int event_len);

void wifi_nrf_wpa_supp_event_proc_deauth_lnx(void *if_priv,
					 struct nrf_wifi_umac_event_mlme *deauth,
					 unsigned int event_len);
#endif//CONFIG_WPA_SUPP

#endif /* __CFH80211_H__ */