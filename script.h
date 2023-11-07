const char _script[] PROGMEM = R"=====(
//helper funs

const d = i => document.getElementById(i);
const ae = (o, e, f) => { o.addEventListener(e, f) };

//global variables
//error messages handler
var ermid = null;
const HOST = window.location.origin;
const TIMEOUT_DELAY = 5e3;
//validate credentials
const vdt = (sid, psd)=>{
    if(sid.length === 0 || psd.length === 0) return false;
    if(!sid.match(/^[^!#;+\]\/"\t][^+\]"\t]{0,31}$/)) return false;
    if(!psd.match(/^[\u0020-\u007e]{8,63}$/)) return false;
    return true;
}
//display error for 3s
const rerr = ()=>{
    const id = 'ID_ERR_MSG';
    const bid = 'ID_ERR_BG';
    const p_ = d(id);
    const b_ = d(bid);
    p_.innerText = "";
    b_.classList.remove('err_m');
    b_.classList.add("d-none");
    ermid = null;
}
const derr = (m) =>{
    const id = 'ID_ERR_MSG';
    const bid = 'ID_ERR_BG';
    const p_ = d(id);
    const b_ = d(bid);
    //add error
    p_.innerText = m;
    if(ermid === null){
        b_.classList.remove('d-none');
        b_.classList.add('err_m');
        ermid = setTimeout(rerr, TIMEOUT_DELAY);
        return;
    }
    clearTimeout(ermid);
    ermid = setTimeout(rerr, TIMEOUT_DELAY);
}

//send credentials via POST

const __pf = (s_,p_) => fetch(`${HOST}/creds`, {
    method: 'POST',
    headers: {
        'content-type' : 'application/json'
    },
    body : JSON.stringify(
        {ssid:s_,
        pass:p_}
    )
}).then(res=>{
    if(!res.ok){
        console.log(`HTTP error! Status : ${res.status}`);
        derr(`HTTP error! Status : ${res.status}`);
    }
    return res.text();
}).then(data=>console.log(data)
).catch(e=>{
    console.log(e);
    derr(e);
})

//submit credentials (e)
const s_creds = () => {
    const ssid_ = `ID_AP_SSID`;
    const pass_ = `ID_AP_PASS`;
    const s_t = d(ssid_);
    const p_t = d(pass_)
    const ssid = s_t.value;
    const pass = p_t.value;
    s_t.value= "";
    p_t.value= "";
    if(!vdt(ssid, pass)){
        derr(`The AP credentials are invalid`);
        return;
    }
    console.log(ssid, pass);
    __pf(ssid, pass);
}
const s_cmd = (cmd, p_, v_) => fetch(v_ === undefined?`${HOST}/${cmd}` : `${HOST}/${cmd}?${p_}=${v_}`)
    .then((res)=>{
        if(!res.ok){
            console.log(`HTTP error! Status : ${res.status}`);
            derr(`HTTP error! Status : ${res.status}`);
        }
        return res.text();
    })
    .then(data=>console.log(data))
    .catch(e=>{
        console.log(e);
        derr(e);
    });

const h_angle_btn = (angle) => {
    d('ID_ANGLE').value = angle;
    d('ID_ANGLE_VALUE').innerText = `${angle}°`;
    s_cmd('angle', 'value', angle);
}

const initialize = () =>{
    const theme = localStorage.getItem('theme');
    const c_dark = theme === 'dark'?  'd-none' : 'theme-svg';
    const c_light = theme === 'dark'?  'theme-svg' : 'd-none';
    d('ID_DARK_IMG').setAttribute('class', c_dark);
    d('ID_LIGHT_IMG').setAttribute('class', c_light);
    const colorTheme = theme === 'dark'? 'dark' : 'light';
    document.documentElement.setAttribute('data-theme', colorTheme);
}

const toggleTheme = (e)=>{
    let _id = e.target.id.substring(3);
    console.log({_id});
    _id = _id.substring(0, _id.indexOf('_')).toLowerCase();
    const colorTheme = localStorage.getItem('theme') === 'dark';
    const theme = colorTheme? 'light' : 'dark';
    localStorage.setItem('theme', theme);
    document.documentElement.setAttribute('data-theme', theme);
    const c_dark = theme === 'dark'?  'd-none' : 'theme-svg';
    const c_light = theme === 'dark'?  'theme-svg' : 'd-none';
    d('ID_DARK_IMG').setAttribute('class', c_dark);
    d('ID_LIGHT_IMG').setAttribute('class', c_light);   
}

const updateAngle = (e) =>{
    d('ID_ANGLE_VALUE').innerText = `${e.target.value}°`;
    const index = parseInt(parseInt(e.target.value) / 15);
    const angle_l = index*15;
    const angle_r = (index + 1) * 15;
    const angle_wrap = d('ID_ANGLE_BTNS');
    angle_wrap.innerHTML = '';
    const btn_l = document.createElement('button');
    const btn_r = document.createElement('button');
    btn_l.setAttribute('class', 'btn');
    btn_r.setAttribute('class', 'btn');
    btn_l.setAttribute('id', 'ID_L_ANGLE');
    btn_r.setAttribute('id', 'ID_R_ANGLE');
    btn_l.setAttribute('onclick', `h_angle_btn(${angle_l})`);
    btn_r.setAttribute('onclick', `h_angle_btn(${angle_r})`);
    btn_l.innerText = `${angle_l}°`;
    btn_r.innerText = `${angle_r}°`;
    angle_wrap.appendChild(btn_l);
    angle_wrap.appendChild(btn_r);
}
//ready
ae(document, 'DOMContentLoaded', () => {
    initialize();
    ae(d('ID_AP_SUBMIT'), 'click', s_creds);
    ae(d('ID_AP_RESET'), 'click', ()=>s_cmd('reset'));
    ae(d('ID_ALL_RESET'), 'click', ()=>s_cmd('rmrf'));
    ae(d('ID_ANGLE'), 'input', updateAngle);
    ae(d('ID_THEME_BTN'), 'click', (e)=>toggleTheme(e));
    ae(d('ID_ANGLE'), 'change', (e)=>s_cmd('angle', 'value', e.target.value));
});







)=====";