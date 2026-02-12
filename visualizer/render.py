import matplotlib.pyplot as plt
import matplotlib.patches as patches

class HeatmapRenderer:
    def __init__(self, layout, trace, cmap="coolwarm"):
        self.layout = layout
        self.trace = trace
        self.cmap = plt.get_cmap(cmap)
        self.vmin, self.vmax = trace.global_minmax()

        self.fig, self.ax = plt.subplots()
        self.ax.set_xlim(0, layout.width)
        self.ax.set_ylim(0, layout.height)
        self.ax.set_aspect("equal")
        self.ax.axis("off")

        self._patch = {}  # (comp,id) -> Rectangle
        for (comp, idx), r in layout.rects.items():
            p = patches.Rectangle((r.x, r.y), r.w, r.h, linewidth=1, edgecolor="black", facecolor="white")
            self.ax.add_patch(p)
            self.ax.text(r.x + r.w/2, r.y + r.h/2, f"{comp}{idx}", ha="center", va="center", fontsize=8)
            self._patch[(comp, idx)] = p

        sm = plt.cm.ScalarMappable(cmap=self.cmap, norm=plt.Normalize(vmin=self.vmin, vmax=self.vmax))
        cb = self.fig.colorbar(sm, ax=self.ax, fraction=0.03, pad=0.02)
        cb.set_label("Temperature")
        self._title = self.ax.text(0.5, 0.98, "", transform=self.ax.transAxes, ha="center", va="top")

    def draw(self, t: int):
        self._title.set_text(f"t = {t}")
        denom = (self.vmax - self.vmin) if (self.vmax > self.vmin) else 1.0
        for (comp, idx), p in self._patch.items():
            val = self.trace.value(comp, idx, t, default=0.0)
            u = (val - self.vmin) / denom
            p.set_facecolor(self.cmap(u))
        return list(self._patch.values()) + [self._title]
