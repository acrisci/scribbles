package main

import (
	"log"

	"golang.org/x/net/context"
	"golang.org/x/oauth2"
	"golang.org/x/oauth2/google"
	_ "google.golang.org/api/oauth2/v2"
    compute "google.golang.org/api/compute/v1"
)

func main() {
    project := "hikerplaces"
	src, err := google.DefaultTokenSource(oauth2.NoContext, "https://www.googleapis.com/auth/cloud-platform")
	if err != nil {
		log.Fatalf("Unable to acquire token source: %v", err)
	}
	client := oauth2.NewClient(context.Background(), src)
    service, err := compute.New(client)
    if err != nil {
        log.Fatalf("Unable to create api service: %v", err)
    }

    regions, err := service.Regions.List(project).Do()
    if err != nil {
        log.Fatalf("Unable to list regions: %v", err)
    }

    println("Regions:")
    for _, region := range regions.Items {
        println(region.Name)
    }

    zones, err := service.Zones.List(project).Do()
    if err != nil {
        log.Fatalf("Unable to list zones: %v", err)
    }

    println("Zones:")
    for _, zone := range zones.Items {
        println(zone.Name)
    }

    println("Instances:")
    for _, zone := range zones.Items {
        instances, err := service.Instances.List(project, zone.Name).Do()
        if err != nil {
            log.Fatalf("Unable to list instances: %v", err)
        }
        for _, instance := range instances.Items {
            println(instance.Name)
        }
    }
}
