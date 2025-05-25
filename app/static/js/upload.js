/* Drag image anywhere on page to upload */
document.addEventListener("dragover", e => e.preventDefault());
document.addEventListener("drop", e => { e.preventDefault(); handleFiles(e.dataTransfer.files); });

const fileInput   = document.getElementById("file-input");
const dropZone    = document.getElementById("drop-zone");
const previewImg  = document.getElementById("preview-img");
const proceedBtn  = document.getElementById("proceed-btn");
const replaceBtn  = document.getElementById("replace-btn");
const startBtn = document.querySelector('label[for="file-input"]');
const dropText = document.querySelector('.drop-text');

/* Browse */
fileInput.addEventListener("change", () => handleFiles(fileInput.files));

/* Highlight drop zone */
["dragenter","dragover"].forEach(evt =>
  dropZone.addEventListener(evt, () => dropZone.classList.add("dragover")));
["dragleave","drop"].forEach(evt =>
  dropZone.addEventListener(evt, () => dropZone.classList.remove("dragover")));

/* Replace image */
replaceBtn.addEventListener("click", () => fileInput.click());

function handleFiles(files) {
  if (!files || !files.length) return;
  const file = files[0];
  if (!file.type.startsWith("image/")) return;

  startBtn.hidden = true;
  dropText.hidden= true;

  // local preview
  previewImg.src = URL.createObjectURL(file);
  previewImg.classList.remove("d-none");

  // show Replace, enable Proceed
  replaceBtn.classList.remove("d-none");
  proceedBtn.classList.add("active");
  proceedBtn.disabled = false;
}