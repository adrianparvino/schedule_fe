import { defineConfig } from "vite";
import melangePlugin from "vite-plugin-melange";
import { visualizer } from 'rollup-plugin-visualizer';

export default defineConfig({
  plugins: [
    melangePlugin({
      emitDir: "src",
      buildCommand: "opam exec -- dune build @react",
      watchCommand: "opam exec -- dune build --watch @react",
    }),
    visualizer(),
  ],
});
