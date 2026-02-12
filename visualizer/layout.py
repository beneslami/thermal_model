import math
from dataclasses import dataclass
from typing import Dict, Tuple

@dataclass(frozen=True)
class Rect:
    x: float
    y: float
    w: float
    h: float

def choose_grid(n: int) -> tuple[int, int]:
    """
    Grid policy:
      - If n <= 8: force 2 rows (2 x ceil(n/2))
      - Else: near-square grid (rows ~ sqrt(n), cols = ceil(n/rows))
    """
    if n <= 0:
        return 1, 1
    if n <= 8:
        rows = 2 if n > 1 else 1
        cols = math.ceil(n / rows)
        return rows, cols

    rows = math.ceil(math.sqrt(n))
    cols = math.ceil(n / rows)

    # optional: keep rows <= cols (wider than tall)
    if rows > cols:
        rows, cols = cols, rows
    return rows, cols

class Layout:
    def __init__(self, rects: Dict[Tuple[str, int], Rect], width: float, height: float):
        self.rects = rects
        self.width = width
        self.height = height

    @staticmethod
    def clustered(counts: Dict[str, int]) -> "Layout":
        """
        Row-aware clustered layout:

        For GPU i:
          - HBM(2i) and HBM(2i+1) are adjacent (side-by-side) and close to GPU i.
          - If GPU is in an "upper" row:   HBM above GPU, PIM above HBM.
          - If GPU is in a "lower" row:   HBM below GPU, PIM below HBM.
        """

        g = counts.get("GPU", 0)
        h = counts.get("HBM", 0)
        p = counts.get("PIM", 0)

        # --- geometry knobs ---
        gpu_w, gpu_h = 28.0, 28.0
        hbm_w, hbm_h = 18.0, 12.0
        pim_w, pim_h = 28.0, 12.0

        # gaps inside a cluster
        gap_intra_y = 4.0
        gap_hbm_pair = 3

        # cluster dimensions
        hbm_pair_w = 2 * hbm_w + gap_hbm_pair
        cluster_w = max(gpu_w, hbm_pair_w, pim_w)
        cluster_h = gpu_h + gap_intra_y + hbm_h + gap_intra_y + pim_h

        # spacing between clusters
        gap_x = 16.0
        gap_y = 16.0

        # place clusters in a near-square grid
        rows, cols = choose_grid(g)

        W = cols * cluster_w + (cols + 1) * gap_x
        H = rows * cluster_h + (rows + 1) * gap_y

        rects: Dict[Tuple[str, int], Rect] = {}

        def center_x(x0: float, w: float) -> float:
            return x0 + (cluster_w - w) / 2.0

        def place_cluster(i: int, x0: float, y0: float, upper_row: bool):
            """
            Cluster local coordinate system: (x0,y0) is bottom-left of the cluster box.
            """
            # Choose vertical arrangement
            if upper_row:
                # bottom -> top: GPU, HBM, PIM
                gy = y0
                hby = gy + gpu_h + gap_intra_y
                py  = hby + hbm_h + gap_intra_y
            else:
                # bottom -> top: PIM, HBM, GPU
                py  = y0
                hby = py + pim_h + gap_intra_y
                gy  = hby + hbm_h + gap_intra_y

            # GPU centered
            gx = center_x(x0, gpu_w)
            rects[("GPU", i)] = Rect(gx, gy, gpu_w, gpu_h)

            # HBM pair centered, adjacent
            h0 = 2 * i
            h1 = 2 * i + 1
            pair_x0 = center_x(x0, hbm_pair_w)
            hb0x = pair_x0
            hb1x = pair_x0 + hbm_w + gap_hbm_pair

            if h0 < h:
                rects[("HBM", h0)] = Rect(hb0x, hby, hbm_w, hbm_h)
            if h1 < h:
                rects[("HBM", h1)] = Rect(hb1x, hby, hbm_w, hbm_h)

            # PIM centered (associated with GPU i)
            if i < p:
                px = center_x(x0, pim_w)
                rects[("PIM", i)] = Rect(px, py, pim_w, pim_h)

        # Define what "upper" means: top half of rows is upper, bottom half is lower.
        # (For rows=2 => row 0 is upper, row 1 is lower.)
        split = rows / 2.0

        for i in range(g):
            r = i // cols           # 0..rows-1 from top to bottom? we'll map to y explicitly
            c = i % cols
            # Make r=0 the top row visually
            y_row = rows - 1 - r
            x0 = gap_x + c * (cluster_w + gap_x)
            y0 = gap_y + y_row * (cluster_h + gap_y)

            # r is "logical from top": r=0 is upper
            upper = (r < split)
            place_cluster(i, x0, y0, upper)

        # If counts don’t match mapping, place extras in a strip to the right (optional)
        extras_x = W + gap_x
        cursor_y = gap_y
        for hid in range(2 * g, h):
            rects[("HBM", hid)] = Rect(extras_x, cursor_y, hbm_w, hbm_h)
            cursor_y += hbm_h + 2.0
        for pid in range(g, p):
            rects[("PIM", pid)] = Rect(extras_x, cursor_y, pim_w, pim_h)
            cursor_y += pim_h + 2.0

        if (h > 2 * g) or (p > g):
            W = extras_x + max(pim_w, hbm_w) + gap_x
            H = max(H, cursor_y + gap_y)

        minx = min(r.x for r in rects.values())
        miny = min(r.y for r in rects.values())
        maxx = max(r.x + r.w for r in rects.values())
        maxy = max(r.y + r.h for r in rects.values())

        bb_w = maxx - minx
        bb_h = maxy - miny

        # choose a canvas that has some padding around the bounding box
        pad = 12.0
        W = bb_w + 2 * pad
        H = bb_h + 2 * pad

        # shift so bounding box center lands at canvas center
        dx = (W / 2.0) - (minx + bb_w / 2.0)
        dy = (H / 2.0) - (miny + bb_h / 2.0)

        rects = {k: Rect(r.x + dx, r.y + dy, r.w, r.h) for k, r in rects.items()}

        return Layout(rects, W, H)
