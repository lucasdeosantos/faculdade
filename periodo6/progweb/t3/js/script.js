const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

const centerX = canvas.width / 2;
const centerY = canvas.height / 2;
let lines = [createLine(centerX - 200, centerY, centerX + 200, centerY)];

const clearButton = document.getElementById("clearButton");

let selectedLine = null;
let selectedPart = null;

const PART = {
    START: "start",
    END: "end",
    CENTER: "center"
};

canvas.addEventListener("mousedown", handleMouseDown);
canvas.addEventListener("mousemove", handleMouseMove);
canvas.addEventListener("mouseup", () => {
    selectedLine = null;
    selectedPart = null;
});
canvas.addEventListener("contextmenu", handleRightClick);

function createLine(x1, y1, x2, y2) {
    return { start: { x: x1, y: y1 }, end: { x: x2, y: y2 } };
}

function draw() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.lineWidth = 2;
    ctx.strokeStyle = "black";

    for (let line of lines) {
        ctx.beginPath();
        ctx.moveTo(line.start.x, line.start.y);
        ctx.lineTo(line.end.x, line.end.y);
        ctx.stroke();

        ctx.fillStyle = "black";
        const radius = 5;

        ctx.beginPath();
        ctx.arc(line.start.x, line.start.y, radius, 0, Math.PI * 2);
        ctx.fill();

        ctx.beginPath();
        ctx.arc(line.end.x, line.end.y, radius, 0, Math.PI * 2);
        ctx.fill();
    }
}

function handleMouseDown(e) {
    const mouse = getMousePos(e);

    for (let line of lines) {
        const part = getLinePartAtPosition(line, mouse);
        if (part) {
            selectedLine = line;
            selectedPart = part;
            break;
        }
    }
}

function handleMouseMove(e) {
    if (!selectedLine || !selectedPart) return;

    const mouse = getMousePos(e);
    if (selectedPart === PART.START) {
        selectedLine.start = mouse;
    } 
    else if (selectedPart === PART.END) {
        selectedLine.end = mouse;
    } 
    else if (selectedPart === PART.CENTER) {
        const dx = mouse.x - (selectedLine.start.x + selectedLine.end.x) / 2;
        const dy = mouse.y - (selectedLine.start.y + selectedLine.end.y) / 2;
        selectedLine.start.x += dx;
        selectedLine.start.y += dy;
        selectedLine.end.x += dx;
        selectedLine.end.y += dy;
    }

    draw();
}

function handleRightClick(e) {
    e.preventDefault();
    const mouse = getMousePos(e);

    for (let i = 0; i < lines.length; i++) {
        const line = lines[i];
        if (isNearLine(line, mouse)) {
            const newLine1 = createLine(line.start.x, line.start.y, mouse.x, mouse.y);
            const newLine2 = createLine(mouse.x, mouse.y, line.end.x, line.end.y);
            lines.splice(i, 1, newLine1, newLine2);
            draw();
            break;
        }
    }
}

function getMousePos(e) {
    const rect = canvas.getBoundingClientRect();
    return {
        x: e.clientX - rect.left,
        y: e.clientY - rect.top
    };
}

function distance(p1, p2) {
    return Math.hypot(p1.x - p2.x, p1.y - p2.y);
}

function getLinePartAtPosition(line, pos) {
    if (distance(pos, line.start) < 10) return PART.START;
    if (distance(pos, line.end) < 10) return PART.END;

    const center = {
        x: (line.start.x + line.end.x) / 2,
        y: (line.start.y + line.end.y) / 2
    };

    if (distance(pos, center) < 40) return PART.CENTER;
    return null;
}

function resetLines() {
    const centerX = canvas.width / 2;
    const centerY = canvas.height / 2;
    lines = [createLine(centerX - 200, centerY, centerX + 200, centerY)];
    draw();
}

function isNearLine(line, pos) {
    const A = line.start;
    const B = line.end;
    const AB = { x: B.x - A.x, y: B.y - A.y };
    const AP = { x: pos.x - A.x, y: pos.y - A.y };
    const ab2 = AB.x * AB.x + AB.y * AB.y;
    const ap_ab = AP.x * AB.x + AP.y * AB.y;
    const t = ap_ab / ab2;

    if (t < 0 || t > 1) return false;

    const closest = {
        x: A.x + AB.x * t,
        y: A.y + AB.y * t
    };

    return distance(pos, closest) < 10;
}

function generatePolygon() {
    const input = document.getElementById("sidesInput");
    const n = parseInt(input.value);

    if (isNaN(n) || n < 3 || n > 8) {
        alert("Digite um número entre 3 e 8.");
        return;
    }

    lines = [];

    const centerX = canvas.width / 2;
    const centerY = canvas.height / 2;
    const radius = 200;

    const angleStep = (2 * Math.PI) / n;
    const points = [];

    for (let i = 0; i < n; i++) {
        const angle = i * angleStep - Math.PI / 2;
        const x = centerX + radius * Math.cos(angle);
        const y = centerY + radius * Math.sin(angle);
        points.push({ x, y });
    }

    for (let i = 0; i < n; i++) {
        const start = points[i];
        const end = points[(i + 1) % n];
        lines.push(createLine(start.x, start.y, end.x, end.y));
    }

    draw();
}

document.getElementById("sidesInput").addEventListener("keydown", function (e) {
    if (e.key === "Enter") {
        generatePolygon();
    }
});

clearButton.addEventListener("click", resetLines);

draw();