sec_dlna_org_extras_none='*;DLNA.ORG_OP=00;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000'

profiles['Samsung-AllShare']=
{
    ['desc']='Samsung AllShare uPnP/DLNA',

    -- SEC_HHP_[TV]UE32ES6757/1.0 DLNADOC/1.50
    ['match']=function(user_agent)
                if string.find(user_agent,'SEC_HHP_',1,true)
                    then return true
                else
                    return false
                end
            end,

    ['options']=
    {
        ['upnp_albumart']=3,     -- <res protocolInfo="http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_TN">http://127.0.0.1:4044/logo?s=0%2F1%2F14%2F33</res>
        ['sec_extras']=true
    },

    ['mime_types']=
    {
        ['mp4']    = { upnp_type.video, upnp_class.video, 'video/mp4',       upnp_proto.mp4,   sec_dlna_org_extras_none },
        ['mkv']    = { upnp_type.video, upnp_class.video, 'video/x-mkv',     upnp_proto.mkv,   sec_dlna_org_extras_none }
    }
}
