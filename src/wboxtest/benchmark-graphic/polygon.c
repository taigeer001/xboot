/*
 * wboxtest/benchmark-graphic/polygon.c
 */

#include <wboxtest.h>

struct wbt_polygon_pdata_t
{
	struct window_t * w;

	ktime_t t1;
	ktime_t t2;
	int calls;
};

static void * polygon_setup(struct wboxtest_t * wbt)
{
	struct wbt_polygon_pdata_t * pdat;

	pdat = malloc(sizeof(struct wbt_polygon_pdata_t));
	if(!pdat)
		return NULL;

	pdat->w = window_alloc(NULL, NULL);
	if(!pdat->w)
	{
		free(pdat);
		return NULL;
	}
	return pdat;
}

static void polygon_clean(struct wboxtest_t * wbt, void * data)
{
	struct wbt_polygon_pdata_t * pdat = (struct wbt_polygon_pdata_t *)data;

	if(pdat)
	{
		window_free(pdat->w);
		free(pdat);
	}
}

static void polygon_run(struct wboxtest_t * wbt, void * data)
{
	struct wbt_polygon_pdata_t * pdat = (struct wbt_polygon_pdata_t *)data;
	struct surface_t * s = pdat->w->s;

	if(pdat)
	{
		srand(0);
		pdat->calls = 0;
		pdat->t2 = pdat->t1 = ktime_get();
		do {
			struct point_t p[10];
			int n = wboxtest_random_int(2, 10);
			for(int i = 0; i < n; i++)
			{
				p[i].x = wboxtest_random_int(0, surface_get_width(s));
				p[i].y = wboxtest_random_int(0, surface_get_height(s));
			}
			struct color_t c;
			color_init(&c, rand() & 0xff, rand() & 0xff, rand() & 0xff, 255);
			int thickness = wboxtest_random_int(0, 50);
			surface_shape_polygon(s, NULL, &p[0], n, thickness, &c);
			pdat->calls++;
			pdat->t2 = ktime_get();
		} while(ktime_before(pdat->t2, ktime_add_ms(pdat->t1, 2000)));
		wboxtest_print(" Counts: %g\r\n", (double)(pdat->calls * 1000.0) / ktime_ms_delta(pdat->t2, pdat->t1));
	}
}

static struct wboxtest_t wbt_polygon = {
	.group	= "benchmark-graphic",
	.name	= "polygon",
	.setup	= polygon_setup,
	.clean	= polygon_clean,
	.run	= polygon_run,
};

static __init void polygon_wbt_init(void)
{
	register_wboxtest(&wbt_polygon);
}

static __exit void polygon_wbt_exit(void)
{
	unregister_wboxtest(&wbt_polygon);
}

wboxtest_initcall(polygon_wbt_init);
wboxtest_exitcall(polygon_wbt_exit);
