import argparse
from .trace import ThermalTrace
from .layout import Layout
from .render import HeatmapRenderer
from .animate import VideoExporter

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--input", required=True, help="thermal output txt file")
    ap.add_argument("--mp4", default="", help="output mp4 path (optional)")
    ap.add_argument("--fps", type=int, default=5)
    ap.add_argument("--show", action="store_true")
    args = ap.parse_args()

    trace = ThermalTrace.from_txt(args.input)
    counts = {comp: len(trace.temps[comp]) for comp in trace.temps}  # infer from file
    layout = Layout.clustered(counts)

    renderer = HeatmapRenderer(layout, trace, cmap="coolwarm")
    exporter = VideoExporter(renderer)

    if args.mp4:
        exporter.save_mp4(args.mp4, fps=args.fps)
    if args.show or not args.mp4:
        exporter.show()

if __name__ == "__main__":
    main()