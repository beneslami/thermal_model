from matplotlib.animation import FuncAnimation

class VideoExporter:
    def __init__(self, renderer):
        self.renderer = renderer

    def show(self, interval_ms=200):
        ani = FuncAnimation(self.renderer.fig,
                            lambda t: self.renderer.draw(t),
                            frames=self.renderer.trace.timesteps,
                            interval=interval_ms,
                            blit=False)
        import matplotlib.pyplot as plt
        plt.show()

    def save_mp4(self, out_path: str, fps: int = 5):
        ani = FuncAnimation(self.renderer.fig,
                            lambda t: self.renderer.draw(t),
                            frames=self.renderer.trace.timesteps,
                            interval=int(1000 / fps),
                            blit=False)
        ani.save(out_path, fps=fps)  # requires ffmpeg installed and visible to matplotlib
