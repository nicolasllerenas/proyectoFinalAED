const buildBtn = document.getElementById("build-btn");
const searchBtn = document.getElementById("search-btn");
const visualization = document.getElementById("visualization");
const searchResult = document.getElementById("search-result");

let currentText = "";

// Construir Suffix Array
buildBtn.addEventListener("click", async () => {
  const text = document.getElementById("input-text").value;
  currentText = text;

  const response = await fetch("http://localhost:5000/build", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ text }),
  });

  const data = await response.json();
  visualizeSuffixArray(data.positions, data.text);
});

// Buscar patrón
searchBtn.addEventListener("click", async () => {
  const pattern = document.getElementById("search-pattern").value;

  const response = await fetch("http://localhost:5000/search", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({
      text: currentText,
      pattern,
    }),
  });

  const data = await response.json();
  showSearchResult(data.position, pattern);
});

// Visualizar Suffix Array
function visualizeSuffixArray(positions, text) {
  visualization.innerHTML = "";

  const table = document.createElement("table");
  table.className = "suffix-table";

  // Cabecera
  const headerRow = document.createElement("tr");
  ["Index", "Position", "Suffix"].forEach((text) => {
    const th = document.createElement("th");
    th.textContent = text;
    headerRow.appendChild(th);
  });
  table.appendChild(headerRow);

  // Filas de datos
  positions.forEach((pos, idx) => {
    const row = document.createElement("tr");

    // Índice
    const idxCell = document.createElement("td");
    idxCell.textContent = idx;
    row.appendChild(idxCell);

    // Posición
    const posCell = document.createElement("td");
    posCell.textContent = pos;
    row.appendChild(posCell);

    // Sufijo
    const suffixCell = document.createElement("td");
    suffixCell.textContent = text.substring(pos);
    suffixCell.className = "suffix";
    row.appendChild(suffixCell);

    table.appendChild(row);
  });

  visualization.appendChild(table);
}

// Mostrar resultado de búsqueda
function showSearchResult(position, pattern) {
  if (position >= 0) {
    searchResult.innerHTML = `
            <div class="success">Pattern "${pattern}" found at position: ${position}</div>
            <div class="highlight">${highlightText(
              currentText,
              position,
              pattern.length
            )}</div>
        `;
  } else {
    searchResult.innerHTML = `<div class="error">Pattern "${pattern}" not found</div>`;
  }
}

// Resaltar texto encontrado
function highlightText(text, start, length) {
  return (
    text.substring(0, start) +
    `<span class="match">${text.substring(start, start + length)}</span>` +
    text.substring(start + length)
  );
}
