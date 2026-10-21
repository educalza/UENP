"use strict";
const $ = id => document.getElementById(id);
let token = sessionStorage.getItem("cluster-key") || "", state = null, editing = null, busy = false, sourceDirty = false;
const fmt = new Intl.NumberFormat("pt-BR", {maximumFractionDigits: 1});
const integer = new Intl.NumberFormat("pt-BR", {maximumFractionDigits: 0});
function speed(n) { for(const [v,u] of [[1e12,"TH/s"],[1e9,"GH/s"],[1e6,"MH/s"],[1e3,"kH/s"]]) if(n>=v) return [fmt.format(n/v),u]; return [fmt.format(n || 0),"H/s"]; }
function rateHTML(n) { const [v,u]=speed(n); return `${v} <small>${u}</small>`; }
function text(tag, value, cls) { const e=document.createElement(tag); e.textContent=value; if(cls)e.className=cls; return e; }
function toast(message) { $("toast").textContent=message; $("toast").hidden=false; clearTimeout(toast.timer); toast.timer=setTimeout(()=>$("toast").hidden=true,5000); }
async function api(path, data) { const r=await fetch(path, {method:data===undefined?"GET":"POST", headers:{Authorization:`Bearer ${token}`,"Content-Type":"application/json"}, ...(data===undefined?{}:{body:JSON.stringify(data)}), signal:AbortSignal.timeout(8000)}); const result=await r.json(); if(!r.ok){if(r.status===401){sessionStorage.removeItem("cluster-key");$("dashboard").hidden=true;$("login").hidden=false;}throw Error(result.erro||`HTTP ${r.status}`);}return result; }
function showDashboard() { $("login").hidden=true;$("dashboard").hidden=false; }
function line(label,value) { const row=text("div","","node-line");row.append(text("span",label),text("b",value));return row; }
function renderNodes() {
  if($("nodes").children.length!==6) {
    $("nodes").replaceChildren();
    for(const n of state.nos) {
      const card=text("article","","node"); card.id=`card-${n.id}`;
      const top=text("div","","node-top"), icon=text("div","","node-icon"), title=text("div",""), label=text("h3",""), id=text("span",n.id.toUpperCase(),"node-id"), status=text("span","","node-state");title.append(label,id);top.append(icon,title,status);
      const rate=text("div","","node-speed"), info=text("div",""), error=text("p","","node-error"), actions=text("div","","node-actions"), pause=text("button",""), adjust=text("button","Ajustar ↗","adjust");
      pause.addEventListener("click",()=>mutate(`/api/nos/${n.id}`,{habilitado:!state.nos.find(x=>x.id===n.id).habilitado}));
      adjust.addEventListener("click",()=>openNode(n.id));actions.append(pause,adjust);card.append(top,rate,info,error,actions);$("nodes").append(card);
    }
  }
  for(const n of state.nos) {
    const card=$(`card-${n.id}`), gpu=n.motor==="cuda", r=n.relato;
    card.querySelector("h3").textContent=n.nome;card.querySelector(".node-icon").textContent=gpu?"▤":"▦";card.querySelector(".node-icon").className=`node-icon${gpu?" gpu":""}`;
    const status=card.querySelector(".node-state"); const label=!n.online?"Desconectado":!n.habilitado?"Pausado":r.erro?"Erro":n.minerando?"Minerando":"Em espera";
    const dot=text("i","",`status-dot ${n.minerando?"green":n.online?"yellow":""}`);status.replaceChildren(dot,document.createTextNode(label));
    card.querySelector(".node-speed").innerHTML=rateHTML(n.minerando?r.velocidade:0);
    card.children[2].replaceChildren(line("Processamento",gpu?(r.gpu||"NVIDIA RTX · CUDA"):(r.motor?`CPU · ${r.motor}`:"CPU · C automático")), line("Sistema",n.online?`${r.sistema||"—"} · ${r.cpus||"—"} CPUs`:"Aguardando conexão"),line("Recursos",gpu?"1 processo · GPU":n.processos?`${n.processos} processos solicitados`:`${r.processos||r.cpus||"Todos os"} processos`));
    card.querySelector(".node-error").textContent=n.online?r.erro||"":"";
    card.querySelector(".node-error").hidden=!(n.online&&r.erro);
    card.querySelector(".node-actions button").textContent=n.habilitado?"Ⅱ Pausar PC":"▶ Ativar PC";
  }
}
function render() {
  $("connection").textContent="Painel conectado";$("connection").classList.remove("offline");
  $("global-control").replaceChildren(document.createTextNode(state.ativo?"Pausar cluster":"Iniciar cluster"),text("span",state.ativo?"Ⅱ":"▶"));
  $("total-rate").innerHTML=rateHTML(state.velocidade);
  const online=state.nos.filter(n=>n.online).length, mining=state.nos.filter(n=>n.minerando).length;
  $("online-count").textContent=online;$("online-detail").textContent=online?`${mining} minerando · ${6-online} desconectados`:"Aguardando os agentes";
  $("accepted").textContent=state.aceitos;$("proof-detail").textContent=state.recusados?`${state.recusados} provas recusadas pela fonte`:"Confirmados pela fonte nesta sessão";
  $("source-state").textContent=!state.ativo?"Pausada":state.fonte_online?"Conectada":"Sem conexão";
  $("source-dot").className=`status-dot ${state.fonte_online&&state.ativo?"green":state.ativo?"yellow":""}`;$("source-address").textContent=state.fonte.replace(/^https?:\/\//,"");
  $("notice").hidden=!(state.ativo&&!state.fonte_online);$("notice").textContent=`Não foi possível consultar a fonte. Os agentes aguardam a reconexão. ${state.erro_fonte}`;
  const w=state.trabalho;$("round-tag").textContent=w?"DISTRIBUÍDO":"EM ESPERA";$("block-label").textContent=w?"Altura do bloco":"Aguardando um bloco";$("block-height").textContent=w?`#${w.altura}`:"—";$("difficulty").textContent=w?`${w.dificuldade} zeros hexadecimais`:"—";$("miner-name").textContent=state.nome;
  $("round-note").textContent=w?"Cada computador percorre uma sequência própria de nonces.":!state.ativo?"Inicie o cluster para buscar trabalhos na fonte.":state.fonte_online?"A fonte ainda não disponibilizou um novo trabalho.":"Tentando reconectar à fonte automaticamente.";
  $("attempts").textContent=`${integer.format(BigInt(state.tentativas))} tentativas registradas`;
  if(!sourceDirty) { $("source-url").value=state.fonte; $("source-name").value=state.nome; }
  $("updated").textContent=`Atualizado às ${new Date().toLocaleTimeString("pt-BR")}`;
  const events=state.eventos.slice(0,8).map(e=>{const li=text("li",""),date=new Date(e.hora*1000);li.append(text("span",e.texto),text("time",date.toLocaleTimeString("pt-BR",{hour:"2-digit",minute:"2-digit",second:"2-digit"})));return li;});$("events").replaceChildren(...events);
  renderNodes();draw();
}
function draw() {
  if(!state||$("dashboard").hidden)return;
  const canvas=$("chart"),box=canvas.getBoundingClientRect(),dpr=window.devicePixelRatio||1;canvas.width=box.width*dpr;canvas.height=box.height*dpr;const c=canvas.getContext("2d");c.scale(dpr,dpr);const width=box.width,height=box.height,pad=42,bottom=height-25,top=12;
  const values=state.historico.map(x=>x.velocidade), max=Math.max(1,...values)*1.2;
  c.font="9px Segoe UI";for(let i=0;i<=3;i++){const y=top+(bottom-top)*i/3;c.strokeStyle="#29323c";c.lineWidth=.5;c.beginPath();c.moveTo(pad,y);c.lineTo(width,y);c.stroke();c.fillStyle="#7b8794";const [v,u]=speed(max*(1-i/3));c.fillText(`${v}${u.replace("/s","")}`,0,y+3);}
  c.fillStyle="#7b8794";c.fillText("−45 s",pad,height-4);c.fillText("agora",Math.max(pad,width-29),height-4);
  $("chart-empty").hidden=values.some(v=>v>0);
  if(values.length<2)return;
  const points=values.map((v,i)=>[pad+(width-pad)*i/(values.length-1),bottom-v/max*(bottom-top)]);
  c.beginPath();points.forEach(([x,y],i)=>i?c.lineTo(x,y):c.moveTo(x,y));c.strokeStyle="#74e4bf";c.lineWidth=2;c.stroke();c.lineTo(width,bottom);c.lineTo(pad,bottom);c.closePath();const gradient=c.createLinearGradient(0,top,0,bottom);gradient.addColorStop(0,"#74e4bf30");gradient.addColorStop(1,"#74e4bf02");c.fillStyle=gradient;c.fill();
}
async function refresh() { if(!token||busy)return;busy=true;try{state=await api("/api/estado");sessionStorage.setItem("cluster-key",token);showDashboard();render();}catch(e){$("connection").textContent="Conexão interrompida";$("connection").classList.add("offline");if(!$("login").hidden)$("login-error").textContent=e.message;else{$("notice").hidden=false;$("notice").textContent="Sem contato com o coordenador. Reconectando…";}}finally{busy=false;} }
async function mutate(path,data){try{await api(path,data);await refresh();toast("Configuração aplicada.");}catch(e){toast(e.message);}}
$("login-form").addEventListener("submit",async e=>{e.preventDefault();token=$("access-key").value.trim();await refresh();});
$("logout").addEventListener("click",()=>{token="";sessionStorage.removeItem("cluster-key");$("dashboard").hidden=true;$("login").hidden=false;$("access-key").value="";});
$("global-control").addEventListener("click",()=>mutate("/api/controle",{acao:state.ativo?"pausar":"iniciar"}));
$("source-form").addEventListener("input",()=>{sourceDirty=true;});
$("source-form").addEventListener("submit",async e=>{e.preventDefault();try{await api("/api/config",{fonte:$("source-url").value.trim(),nome:$("source-name").value.trim()});sourceDirty=false;await refresh();toast("Fonte atualizada.");}catch(err){toast(err.message);}});
function openNode(id){editing=id;const n=state.nos.find(x=>x.id===id);$("node-dialog-title").textContent=`Ajustar ${n.nome}`;$("node-name").value=n.nome;$("node-engine").value=n.motor;$("node-processes").value=n.processos;$("node-dialog").showModal();}
$("node-form").addEventListener("submit",async e=>{e.preventDefault();try{await api(`/api/nos/${editing}`,{nome:$("node-name").value.trim(),motor:$("node-engine").value,processos:Number($("node-processes").value)});$("node-dialog").close();await refresh();toast("Ajustes enviados ao computador.");}catch(err){toast(err.message);}});
function openSetup(){if(!$("coordinator-url").value)$("coordinator-url").value=["127.0.0.1","localhost"].includes(location.hostname)?(state.enderecos_painel?.[0]||location.origin):location.origin;$("downloads").replaceChildren(...state.nos.map(n=>{const b=text("button",`↓ ${n.nome}`,"secondary");b.addEventListener("click",()=>download(n.id));return b;}));$("setup").showModal();}
for(const id of ["setup-open","setup-open-2"])$(id).addEventListener("click",openSetup);
for(const button of document.querySelectorAll(".close-dialog"))button.addEventListener("click",()=>button.closest("dialog").close());
async function download(id){try{const url=new URL($("coordinator-url").value);if(!["http:","https:"].includes(url.protocol)||url.username||url.password)throw Error("Informe um endereço HTTP válido.");if(["localhost","127.0.0.1","0.0.0.0","[::1]"].includes(url.hostname))throw Error("Informe o IP da rede local deste computador para conectar os outros PCs.");const config=await api(`/api/configuracao/${id}`);config.coordenador=url.origin;const blob=new Blob([JSON.stringify(config,null,2)],{type:"application/json"}),link=document.createElement("a"),ref=URL.createObjectURL(blob);link.href=ref;link.download=`${id}.json`;link.click();setTimeout(()=>URL.revokeObjectURL(ref),1000);toast(`Configuração ${id}.json baixada.`);}catch(e){toast(e.message);}}
window.addEventListener("resize",draw);refresh();setInterval(refresh,1000);
